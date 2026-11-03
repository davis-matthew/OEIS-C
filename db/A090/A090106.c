/*
 * A090106  Values of k such that {P(k), P(k+1), ..., P(k+12)} are all prime
 *          numbers, where P(k) = k^2 + k + 41.
 * https://oeis.org/A090106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A090106:  k such that P(k)=k²+k+41, …, P(k+12) are all prime.
   The function returns the k‑th such value (a(1)=1). It uses a simple
   brute‑force search with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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
    /* Static cache of found k-values (stored as seq_t). */
    static seq_t *cache     = NULL; /* array of values */
    static size_t cache_len = 0;    /* number of cached entries      */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t next_k = 1;        /* next k to test               */

    /* Grow the cache until it contains at least n entries. */
    while (cache_len < n) {
        bool all_prime = true;

        /* Test the 13 values of P(k+j) for j = 0 … 12. */
        for (int j = 0; j <= 12 && all_prime; ++j) {
            idx_t x = next_k + j;

            /* Compute P(x) = x² + x + 41 using seq_t arithmetic.
               This works for both built‑in integer types and for the
               arbitrary‑precision seq_t classes that overload + and *.
            */
            seq_t sx   = (seq_t)x;
            seq_t val  = sx * sx + sx + (seq_t)41;

            if (!is_prime_big(val))
                all_prime = false;
        }

        if (all_prime) {
            /* Ensure the cache has space for the new entry. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)
                    abort();            /* allocation failure */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
