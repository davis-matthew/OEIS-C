/*
 * A036589  Number of binary rooted trees with n nodes and height at most 6.
 * https://oeis.org/A036589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let T[h][n] be the number of unordered rooted binary trees
   with n nodes and height ≤ h.  Then
        T[0][0] = 1,        T[0][n>0] = 0
        T[h][0] = 1   for h ≥ 0
        T[h][n] = Σ_{i=0}^{⌊(n−1)/2⌋}  T[h−1][i] * T[h−1][n−1−i]      (i < n−1−i)
                 + ( T[h−1][i] * (T[h−1][i] + 1) ) / 2                  (i = n−1−i)
   The desired sequence is T[6][n].  This implementation computes all values
   up to the requested n on first use and memoises them for subsequent calls. */
#include <stddef.h>
#include <stdlib.h>

#define MAX_RECURSION 200   /* sufficient for height 6 (max 127 nodes) */

/* Static tables for heights 0..6 and nodes 0..MAX_RECURSION. */
static seq_t T[7][MAX_RECURSION + 1];
static idx_t max_computed = 0;

static void initialise(void)
{
    /* Base cases */
    T[0][0] = (seq_t)1;
    for (int i = 1; i <= MAX_RECURSION; ++i)
        T[0][i] = (seq_t)0;
    for (int h = 1; h <= 6; ++h)
        T[h][0] = (seq_t)1;
}

static void compute_upto(idx_t n)
{
    if (n <= max_computed)
        return;
    if (n > MAX_RECURSION)
        n = MAX_RECURSION;   /* values beyond this are trivially zero */

    for (int h = 1; h <= 6; ++h) {
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t sum = 0;
            idx_t limit = (k - 1) / 2;
            for (idx_t i = 0; i <= limit; ++i) {
                idx_t j = k - 1 - i;
                seq_t ai = T[h - 1][i];
                seq_t bj = T[h - 1][j];
                if (i == j) {
                    seq_t term = ai * (ai + (seq_t)1);
                    term /= 2;
                    sum += term;
                } else {
                    sum += ai * bj;
                }
            }
            T[h][k] = sum;
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (max_computed == 0)
        initialise();
    if (n > MAX_RECURSION) {         /* no such tree if height ≤ 6 */
        return (seq_t)0;
    }
    compute_upto(n);
    return T[6][n];
}
