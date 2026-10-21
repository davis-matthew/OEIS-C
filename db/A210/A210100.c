/*
 * A210100  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          one or three distinct values, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A210100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) + 2*a(n-3), with a(1)=7, a(2)=30,
 *            a(3)=135
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is 1‑based; index 0 is unused. */
    static seq_t *seq = NULL;      /* cache of computed values */
    static idx_t sz   = 0;         /* current length of cache (max index+1) */

    if (n == 0) return (seq_t)0;   /* behaviour below offset irrelevant */

    /* Ensure the cache can hold index n. */
    if (sz <= n) {
        idx_t new_sz = n + 1;                     /* indices 0..n */
        seq_t *tmp = (seq_t*)realloc(seq, new_sz * sizeof(seq_t));
        if (tmp == NULL) {                       /* allocation failure */
            free(seq);
            seq = NULL;
            sz  = 0;
            return (seq_t)0;
        }
        seq = tmp;
        sz  = new_sz;
    }

    /* Initialise the first three terms – harmless if they were already set. */
    if (sz > 1) seq[1] = (seq_t)7;
    if (sz > 2) seq[2] = (seq_t)30;
    if (sz > 3) seq[3] = (seq_t)135;

    /* Build up to n using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = seq[i-1] * (seq_t)6;
        seq_t t2 = seq[i-2] * (seq_t)7;
        seq_t t3 = seq[i-3] * (seq_t)2;
        seq[i]   = t1 + t3 - t2;  /* 6*a(n-1) - 7*a(n-2) + 2*a(n-3) */
    }

    return seq[n];
}
