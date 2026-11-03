/*
 * A154550  Greater of two consecutive primes, p < q, such that p*q+p-q is
 *          prime.
 * https://oeis.org/A154550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th larger prime q of consecutive primes (p,q) such
 *            that p*q + p - q is prime. The function searches successive
 *            pairs of prime numbers (p,q) in increasing order, tests the
 *            expression, and records q whenever the expression is prime. The
 *            n‑th stored q is returned
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, return 0 */

    static uint64_t last_prime = 2;        /* last prime processed */

#ifndef __cplusplus
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
#else
    static std::vector<seq_t> cache;
#endif

    /* Generate terms until we have at least n of them */
    while (1)
    {
#ifndef __cplusplus
        if (cache_len >= (size_t)n) break;
#else
        if (cache.size() >= (size_t)n) break;
#endif
        uint64_t p = last_prime;
        uint64_t q = next_prime(p);      /* next prime after p */
        last_prime = q;                  /* advance for next iteration */

        seq_t expr = (seq_t)p * (seq_t)q + (seq_t)p - (seq_t)q;

        if (is_prime_big(expr))           /* condition satisfied */
        {
#ifndef __cplusplus
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)q;
#else
            cache.push_back((seq_t)q);
#endif
        }
    }

    /* Return the nth term (1‑based) */
#ifndef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
