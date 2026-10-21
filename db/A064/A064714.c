/*
 * A064714  Define q(0)=1, q(1)=1, q(2)=1,
 *          q(n)=q(abs(n-q(n-2)))+q(abs(n-q(n-3))) (A063892); then a(0) = 1,
 *          a(n)=a(n-1)+2*(q(n)-n/2), n > 0.
 * https://oeis.org/A064714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*q(n) - n (n > 0) with q(0)=q(1)=q(2)=1,
 *            q(n)=q(|n-q(n-2)|)+q(|n-q(n-3)|) (n≥3)
 * Range:     u8 n <= 51, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   abs_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
#include <stdlib.h>
#include "../utils/abs_diff.h"

/* Cached values of q and a.  The arrays grow as needed.  */
static seq_t *q_arr   = NULL;
static seq_t *a_arr   = NULL;
static idx_t  cache_sz = 0;   /* highest computed index + 1 */

/* Grow the caches to cover at least index n.  */
static void ensure_upto(idx_t n)
{
    if (n < cache_sz) return;

    /* Determine new capacity (double until enough). */
    idx_t new_cap = cache_sz ? cache_sz << 1 : 8;
    while (new_cap <= n) new_cap <<= 1;

    /* Reallocate with proper casts for C++. */
    q_arr = (seq_t *)realloc(q_arr, new_cap * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));

    /* Compute missing terms. */
    for (idx_t i = cache_sz; i <= n; ++i)
    {
        if (i == 0)          /* base case */
        {
            q_arr[0] = 1;
            a_arr[0] = 1;
        }
        else if (i <= 2)     /* q(1)=q(2)=1 */
        {
            q_arr[i] = 1;
            /* a(i) = a(i-1) + 2*q(i) - i */
            a_arr[i] = a_arr[i-1] + ((seq_t)2 * q_arr[i]) - (seq_t)i;
        }
        else                 /* recurrence for q and a */
        {
            idx_t idx1 = abs_diff(i, q_arr[i-2]);
            idx_t idx2 = abs_diff(i, q_arr[i-3]);
            q_arr[i] = q_arr[idx1] + q_arr[idx2];

            a_arr[i] = a_arr[i-1] + ((seq_t)2 * q_arr[i]) - (seq_t)i;
        }
    }

    cache_sz = n + 1;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return a_arr[n];
}
