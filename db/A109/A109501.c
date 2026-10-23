/*
 * A109501  Number of closed walks of length n on the complete graph on 7
 *          nodes from a given node.
 * https://oeis.org/A109501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^n + 6*(-1)^n) / 7 Recurrence: a(0) = 1, a(1) = 0, a(n)
 *            = 5*a(n-1) + 6*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* memoisation array */
static idx_t seq_len = 0;          /* number of stored terms */

/* Ensure that seq[n] is available */
static void ensure(idx_t n)
{
    if (n < seq_len)
        return;                    /* already computed */

    /* allocate larger array */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
    if (!tmp)                     /* allocation failed, abort */
        return;

    /* initialise new entries */
    if (seq_len == 0) {            /* first run: set a(0) and a(1) */
        tmp[0] = (seq_t)1;
        if (new_len > 1)
            tmp[1] = (seq_t)0;
        seq_len = 2;
    }

    /* fill remaining entries using the recurrence */
    for (idx_t i = seq_len; i <= n; ++i) {
        if (i == 0) {
            tmp[0] = (seq_t)1;
        } else if (i == 1) {
            tmp[1] = (seq_t)0;
        } else {
            tmp[i] = 5 * tmp[i - 1] + 6 * tmp[i - 2];
        }
    }

    seq_len = new_len;
    seq = tmp;
}

seq_t a(idx_t n)
{
    ensure(n);
    return seq[n];
}
