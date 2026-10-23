/*
 * A105926  First differences of A000166.
 * https://oeis.org/A105926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = subfactorial(n+1) – subfactorial(n) subfactorial
 *            recurrence: D(0) = 1 D(1) = 0 D(k) = (k-1) * ( D(k-1) + D(k-2) )
 *            Thus a(n) = D(n+1) - D(n)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *D = NULL;          /* cache of subfactorials */
static idx_t  D_size = 0;       /* highest index stored + 1 */
static idx_t  D_cap  = 0;       /* allocated capacity */

/* Ensure that D has at least indices 0..need inclusive */
static void ensure_D(idx_t need)
{
    if (need + 1 <= D_size) return;          /* already present */

    if (D == NULL) {
        D_cap = 2;
        D = (seq_t *)malloc(D_cap * sizeof(seq_t));
        D[0] = (seq_t)1;   /* D(0) = 1 */
        D[1] = (seq_t)0;   /* D(1) = 0 */
        D_size = 2;
    }
    /* grow capacity */
    if (need + 1 > D_cap) {
        idx_t new_cap = D_cap;
        while (new_cap <= need + 1) new_cap <<= 1;
        D = (seq_t *)realloc(D, new_cap * sizeof(seq_t));
        D_cap = new_cap;
    }
    /* compute missing values */
    for (idx_t i = D_size; i <= need; ++i) {
        /* D(i) = (i-1) * (D(i-1) + D(i-2)) */
        seq_t sum = D[i-1] + D[i-2];
        D[i] = (seq_t)(i-1) * sum;
    }
    D_size = need + 1;
}

seq_t a(idx_t n)
{
    /* we need subfactorials up to n+1 */
    ensure_D(n + 1);
    return D[n + 1] - D[n];
}
