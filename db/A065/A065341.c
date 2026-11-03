/*
 * A065341  Mersenne composites: 2^prime(m) - 1 is not a prime.
 * https://oeis.org/A065341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 98 (2026-11-03)
 * Helpers:   is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065341: a(n) = 2^p - 1 where p is the n‑th prime that makes 2^p-1 composite. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache of previously found Mersenne composites */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* next integer to test for primality (prime candidates) */
    static uint64_t next_candidate = 2;

    if (n == 0)          /* offset starts at 1, n = 0 is unused */
        return (seq_t)0;

    /* fill cache until it holds at least n terms */
    while (cache_len < n)
    {
        /* find next prime number p */
        uint64_t p;
        while (true)
        {
            if (is_prime_big((seq_t)next_candidate))
            {
                p = next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }

        /* compute 2^p - 1 */
        seq_t mersenne = pow2((idx_t)p);
        mersenne -= (seq_t)1;            /* subtract 1 */

        /* keep if it is composite */
        if (!is_prime_big(mersenne))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                if (cache == NULL) abort();   /* allocation failure */
                cache_cap = new_cap;
            }
            cache[cache_len++] = mersenne;
        }
    }

    return cache[n - 1];
}
