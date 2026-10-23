/*
 * A117789  Lucas numbers which are divisible by the sum of their digits.
 * https://oeis.org/A117789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Lucas numbers L such that L % digit_sum(L) == 0.
   The function generates Lucas numbers in order, checks the
   divisibility condition, caches the qualifying ones, and
   returns the nth qualifying value. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static state for caching qualifying Lucas numbers */
    static bool initialized = false;
    static seq_t *good_cache = NULL;
    static size_t good_cap   = 0;
    static size_t good_count = 0;

    static seq_t L_prev = 0;   /* L(k-1) */
    static seq_t L_curr = 0;   /* L(k)   */
    static idx_t  last_k  = 0; /* index of L_curr */

    if (!initialized) {
        /* First two Lucas numbers that satisfy the condition:
           L1 = 1, L2 = 3 (both are divisible by their digit sums). */
        L_prev = (seq_t)1;
        L_curr = (seq_t)3;
        last_k  = 2;

        good_cap   = 4;
        good_cache = (seq_t *)malloc(good_cap * sizeof(seq_t));
        if (!good_cache) { /* allocation failure, return 0 as fallback */
            return (seq_t)0;
        }

        good_cache[0] = L_prev;
        good_cache[1] = L_curr;
        good_count  = 2;

        initialized = true;
    }

    while (good_count < n) {
        /* Next Lucas number: L(k+1) = L(k-1) + L(k) */
        seq_t next = L_prev + L_curr;
        L_prev = L_curr;
        L_curr = next;
        ++last_k;

        seq_t ds = digit_sum(next);
        if (ds != (seq_t)0 && (next % ds) == (seq_t)0) {
            if (good_count == good_cap) {
                good_cap = good_cap * 2;
                seq_t *tmp = (seq_t *)realloc(good_cache, good_cap * sizeof(seq_t));
                if (!tmp) { /* realloc failure, return 0 as fallback */
                    return (seq_t)0;
                }
                good_cache = tmp;
            }
            good_cache[good_count++] = next;
        }
    }

    return good_cache[n - 1];
}
