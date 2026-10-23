/*
 * A158845  Numbers k such that prepending 1 to the k-th triangular number
 *          produces a prime.
 * https://oeis.org/A158845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that concatenating 1 and the k-th
 *            triangular number T(k) = k(k+1)/2 yields a prime
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* static cache of already found terms */
    static seq_t *cache = NULL;        /* array of found k's */
    static size_t cache_size = 0;      /* current number of terms stored */
    static size_t cache_cap  = 0;      /* capacity of the cache */
    static idx_t  next_k     = 1;      /* next candidate k to test */

    if (!cache) {                      /* initialize on first call */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    while (cache_size < (size_t)n) {   /* generate until we have n terms */
        seq_t k = (seq_t)next_k;
        seq_t T = k * (k + 1) / 2;               /* triangular number */
        int d = digit_len(T);                    /* decimal digit count of T */
        seq_t candidate = pow10((idx_t)d) + T;    /* concatenate 1 and T */

        if (isPrimeGeneric(candidate)) {
            if (cache_size == cache_cap) {       /* grow cache if needed */
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = k;             /* store found k */
        }
        next_k++;                                 /* try next k */
    }

    return cache[n - 1];                          /* 1‑based indexing */
}
