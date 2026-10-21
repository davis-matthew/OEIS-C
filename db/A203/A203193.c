/*
 * A203193  (n-1)-st elementary symmetric function of the first n terms of
 *          (1,2,1,3,1,4,1,5,...)=A133622.
 * https://oeis.org/A203193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b_n * a(n-1) + P_{n-1} where b_n = 1 if n odd, otherwise
 *            n/2 + 1, P_{n-1} = product of the first (n-1) terms of b
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the utilities seq_alloc.h and seq_realloc.h */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static storage for intermediate values and memoized results */
    static seq_t *a_arr   = NULL;   /* index 1.. */
    static size_t arr_cap = 0;      /* number of elements allocated */
    static idx_t  up_to   = 0;      /* highest n computed so far */
    static seq_t  prod    = 1;      /* product of first up_to terms (P_up_to) */
    static seq_t  a_val   = 0;      /* a(up_to) */

    /* allocate array if not yet allocated */
    if (!a_arr) {
        arr_cap = 16;
        a_arr = seq_alloc(arr_cap);  /* a_arr[0] unused; indices 1..arr_cap-1 */
    }

    /* extend array capacity if required */
    if (n >= arr_cap) {
        size_t new_cap = arr_cap;
        while (n >= new_cap) new_cap *= 2;
        a_arr = seq_realloc(a_arr, arr_cap, new_cap);
        arr_cap = new_cap;
    }

    /* if we already have the value, return it immediately */
    if (n <= up_to) return a_arr[n];

    /* compute sequentially from up_to + 1 up to n */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        seq_t b = (i & 1) ? (seq_t)1 : (seq_t)(i / 2 + 1);
        seq_t new_a = b * a_val + prod;   /* a(i) = b_i * a(i-1) + P_{i-1} */
        prod = prod * b;                  /* update product for next step */
        a_val = new_a;                    /* store a(i) for reuse */
        a_arr[i] = new_a;                 /* memoize this term */
    }

    up_to = n;
    return a_arr[n];
}
