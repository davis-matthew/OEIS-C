/*
 * A009211  Expansion of exp(sin(x)*cosh(x)).
 * https://oeis.org/A009211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 For n ≥ 1, n * a(n) = Σ_{k odd} k * w(k) * C(n,k) *
 *            a(n-k) where w(1) = 1, w(3) = 2, w(k+4) = -4 * w(k) for odd k (k
 *            ≥ 1). (This yields w(k)=(-1)^{⌊(k-1)/4⌋}·2^{(k-1)/2}). C(n,k) is
 *            the binomial coefficient. All arithmetic is performed in seq_t
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   resize_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/resize_seq_arr.h"

static seq_t *a_arr = NULL;          /* memoized a(n) values  */
static idx_t  a_cap = 0;             /* allocated size of a_arr   */
static idx_t  a_last = (idx_t)-1;    /* highest index computed  */

static seq_t *w_arr = NULL;          /* w(k) values (k odd)      */
static idx_t  w_cap = 0;             /* allocated size of w_arr   */
static idx_t  w_last = (idx_t)-1;    /* highest index computed  */

/* Ensure that a_arr and w_arr are computed up to index n. */
static void ensure_cache(idx_t n)
{
    /* First compute w(k) for all odd k ≤ n. */
    if (n >= w_cap) {
        w_arr = resize_seq_arr(w_arr, &w_cap, n);
        /* Initialize new entries to 0 (important for even indices). */
        for (idx_t i = w_last + 1; i <= n; ++i) w_arr[i] = (seq_t)0;
        for (idx_t k = w_last + 1; k <= n; ++k) {
            if ((k & 1) == 0) continue;          /* even k -> w(k)=0 */
            if (k == 1)           w_arr[k] = (seq_t)1;
            else if (k == 3)      w_arr[k] = (seq_t)2;
            else                  w_arr[k] = -(w_arr[k - 4] * (seq_t)4);
        }
        w_last = n;
    }

    /* Next compute a(n) for all n ≤ n. */
    if (n >= a_cap) {
        a_arr = resize_seq_arr(a_arr, &a_cap, n);
        if (a_last == (idx_t)-1) {      /* first time */
            a_arr[0] = (seq_t)1;
            a_last = 0;
        }
        for (idx_t i = a_last + 1; i <= n; ++i) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;                 /* C(i,0) */
            for (idx_t k = 1; k <= i; ++k) {
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;  /* update to C(i,k) */
                if (k & 1) {                      /* odd k only */
                    seq_t term = a_arr[i - k];
                    term = term * w_arr[k];
                    term = term * (seq_t)k;
                    term = term * binom;
                    sum  = sum + term;
                }
            }
            a_arr[i] = sum / (seq_t)i;
        }
        a_last = n;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return a_arr[n];
}
