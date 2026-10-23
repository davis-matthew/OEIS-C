/*
 * A117994  Heptagonal numbers with only even digits.
 * https://oeis.org/A117994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   all_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Heptagonal numbers with only even digits
   a(n) = the n-th H_k = k(5k‑3)/2 such that every decimal digit of H_k is even.
   Computation: scan k=0,1,2,…, testing each H_k for even‑digit property
   and cache the results; memoization allows arbitrary call order. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/all_even_digits.h"

seq_t a(idx_t n)
{
    /* static cache for the found Heptagonal numbers */
    static seq_t *cache   = NULL;      /* array of found Heptagonal numbers */
    static size_t  capacity = 0;      /* allocated size of cache      */
    static size_t  length   = 0;      /* number of terms stored       */
    static idx_t   next_k   = 0;      /* next index k to evaluate    */

    if (n == 0) {          /* offset is 1, but handle 0 safely */
        return 0;
    }

    /* extend the cache until it contains at least n terms */
    while (length < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t H = k * ((seq_t)5 * k - 3) / 2;   /* Heptagonal number H_k */

        if (all_even_digits(H)) {
            if (length == capacity) {          /* need more room */
                size_t newcap = (capacity == 0) ? 8 : capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) exit(1);           /* fatal error */
                cache = tmp;
                capacity = newcap;
            }
            cache[length++] = H;
        }
        ++next_k;
    }

    return cache[n - 1];   /* a(1) corresponds to cache[0] */
}
