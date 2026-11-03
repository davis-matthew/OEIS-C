/*
 * A051648  Primes of form 210*p + 1 where p is a prime.
 * https://oeis.org/A051648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9841 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051648: primes of the form 210*p + 1, where p is a prime.
   Computed by searching successive primes p, testing whether 210*p+1
   is prime, and caching the resulting values. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n) {
    /* static cache of results (1‑based index) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated size of cache */
    static size_t sz    = 0;     /* number of terms stored   */
    static idx_t  next_p = 2;    /* next candidate prime p  */

    if (n == 0) {
        /* offset is 1; n==0 is not used, return 0 as sentinel */
        return (seq_t)0;
    }

    /* generate terms until we have at least n of them */
    while (sz < (size_t)n) {
        /* find next prime p */
        idx_t p = next_p;
        while (!is_prime(p)) {
            ++p;
        }
        next_p = p + 1;          /* start after this prime next time */

        /* candidate q = 210*p + 1 */
        seq_t q = (seq_t)210 * (seq_t)p + (seq_t)1;

        /* if q is prime, cache it */
        if (is_prime_big(q)) {
            if (sz == cap) {     /* grow cache */
#ifdef __cplusplus
                size_t new_cap = (cap == 0) ? 1 : cap * 2;
                seq_t *tmp = new seq_t[new_cap];
                for (size_t i = 0; i < sz; ++i)
                    tmp[i] = cache[i];
                delete[] cache;
                cache = tmp;
                cap   = new_cap;
#else
                size_t new_cap = (cap == 0) ? 1 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
#endif
            }
            cache[sz++] = q;
        }
        /* otherwise, discard q and continue searching */
    }

    /* return requested term (1‑based) */
    return cache[(size_t)n - 1];
}
