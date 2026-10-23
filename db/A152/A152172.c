/*
 * A152172  a(n) is the number of Dyck paths of semilength n without height of
 *          peaks 0 (mod 3) and height of valleys 1 (mod 3).
 * https://oeis.org/A152172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence (n+1)*a(n) = 2*n*a(n-1) -
 *            (7-3*n)*a(n-2) - 4*a(n-3) - 4*(n-4)*a(n-4) with a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3. Implementation uses memoisation
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current allocated size */
    static size_t last  = 0;   /* number of elements already computed (0..last-1) */

    /* Grow the cache if necessary. */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_arr = alloc_seq_array(new_cap);
        if (!new_arr) abort();                     /* allocation failed */
        /* copy existing values */
        for (size_t i = 0; i < last; ++i)
            new_arr[i] = cache[i];
        /* free old array */
        if (cache) free_seq_array(cache);
        cache = new_arr;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = last; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)2;
        } else if (i == 3) {
            cache[3] = (seq_t)3;
        } else {
            seq_t i_seq = (seq_t)i;
            seq_t t1 = cache[i-1];   /* a(n-1) */
            seq_t t2 = cache[i-2];   /* a(n-2) */
            seq_t t3 = cache[i-3];   /* a(n-3) */
            seq_t t4 = cache[i-4];   /* a(n-4) */

            /* Compute 2*n*a(n-1). */
            seq_t sum_pos = (seq_t)2 * i_seq * t1;   /* positive part */

            /* Compute coefficient (7-3*n) and decide sign. */
            int64_t coeff = 7 - 3 * (int64_t)i;       /* may be negative */
            seq_t abs_coeff = (seq_t)(coeff >= 0 ? coeff : -coeff);

            if (coeff >= 0) {
                /* subtract abs_coeff * a(n-2) */
                sum_pos += 0; /* no change to positive sum */
                seq_t term_neg = abs_coeff * t2;
                /* accumulate into negative part first */
                seq_t sum_neg = term_neg;
                /* add the -4*a(n-3) and -4*(n-4)*a(n-4) terms */
                sum_neg += (seq_t)4 * t3;
                sum_neg += (seq_t)4 * (i_seq - 4) * t4;
                /* Now subtract the negative part from the positive sum */
                seq_t num = sum_pos - sum_neg;
                cache[i] = num / (i_seq + 1);
            } else {
                /* add abs_coeff * a(n-2) to the positive sum */
                sum_pos += abs_coeff * t2;
                /* negative part consists of the two -4 terms only */
                seq_t sum_neg = (seq_t)0;
                sum_neg += (seq_t)4 * t3;
                sum_neg += (seq_t)4 * (i_seq - 4) * t4;
                seq_t num = sum_pos - sum_neg;
                cache[i] = num / (i_seq + 1);
            }
        }
    }

    last = (size_t)n + 1;
    return cache[n];
}
