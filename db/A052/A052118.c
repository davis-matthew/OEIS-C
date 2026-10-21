/*
 * A052118  Number of nonnegative integer pairs (i,j) with binomial(i+r,r) +
 *          binomial(j+r,r) <= binomial(n+r,r), r=5.
 * https://oeis.org/A052118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Counting pairs (i,j) with C(i+5,5)+C(j+5,5) <= C(n+5,5)
 *
 *  The algorithm uses the monotone array
 *      f[k] = C(k+5,5)  (k = 0,1,2,...)
 *  For each n it counts the solutions by a two‑pointer sweep on the subarray
 *  f[0..n].  Results are memoised so that non‑monotonic calls are handled
 *  efficiently.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

static seq_t *f_arr = NULL;   /* f[k] = binomial(k+5,5)   */
static idx_t  f_len = 0;      /* number of f entries stored    */
static seq_t *a_arr = NULL;   /* a[n] counts                    */
static idx_t  a_len = 0;      /* length of a_arr                */

seq_t a(idx_t n)
{
    /* Ensure we have f values up to n */
    if (n + 1 > f_len) {
        size_t new_len = (size_t)(n + 1);
        f_arr = (seq_t *)realloc(f_arr, new_len * sizeof(seq_t));
        if (!f_arr) abort();
        for (idx_t i = f_len; i <= n; ++i)
            f_arr[i] = binomial(i + (idx_t)5, (idx_t)5);
        f_len = new_len;
    }

    /* Compute a[n] for any indices that haven't been computed yet */
    if (n + 1 > a_len) {
        size_t new_len = (size_t)(n + 1);
        a_arr = (seq_t *)realloc(a_arr, new_len * sizeof(seq_t));
        if (!a_arr) abort();
        for (idx_t k = a_len; k <= n; ++k) {
            seq_t target = f_arr[k];
            long long j = k;           /* signed to allow -1 */
            seq_t count = 0;

            for (idx_t i = 0; i <= k; ++i) {
                /* Move j down until sum <= target */
                while (j >= 0 && f_arr[i] + f_arr[(idx_t)j] > target)
                    --j;
                if (j < 0) break;
                count += (seq_t)(j + 1);   /* j+1 values of j satisfy the inequality */
            }
            a_arr[k] = count;
        }
        a_len = new_len;
    }
    return a_arr[n];
}
