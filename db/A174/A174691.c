/*
 * A174691  A symmetrical triangle sequence: t(n,m)=n!*Eulerian[n + 1, m] - n!
 *          + 1.
 * https://oeis.org/A174691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = t(k, m) where k is the row and m the column in the
 *            triangular array t(n,m) = n!*Eulerian[n + 1, m] - n! + 1,
 *            flattened row by row starting with a(0)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 39, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   factorial, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- standard headers ------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorial.h" /* seq_t factorial(idx_t n) */
#include "../utils/seq_one.h"   /* seq_t seq_one(void) */

/* ---------- Eulerian numbers by recurrence ----------------------------- */
/* 0 <= k <= n-1 */
static seq_t
eulerian(idx_t n, idx_t k)
{
    if (k == 0)
        return seq_one();        /* E(n,0) = 1 for all n >= 1 */

    /* allocate two rows of size k+1 */
    seq_t *prev = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return seq_one();        /* unreachable in normal use */
    }

    /* row 0: E(0,0) = 1, others zero */
    prev[0] = seq_one();
    for (idx_t j = 1; j <= k; ++j)
        prev[j] = 0;

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = seq_one();          /* E(i,0) = 1 */
        for (idx_t j = 1; j <= k; ++j) {
            if (j > i) {             /* E(i,j) = 0 when j > i-1 */
                cur[j] = 0;
                continue;
            }
            seq_t t1 = 0;
            if (j <= i - 1)
                t1 = (seq_t)(j + 1) * prev[j];
            seq_t t2 = (seq_t)(i - j) * prev[j - 1];
            cur[j] = t1 + t2;
        }
        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* result is in prev[k] after final iteration */
    seq_t result = prev[k];
    free(prev);
    free(cur);
    return result;
}

/* ---------- main sequence generator ----------------------------------- */
seq_t
a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    idx_t cum = 1; /* sum of rows up to r */
    while (cum <= n) {
        ++r;
        cum += r + 1;
    }
    idx_t prev_cum = cum - (r + 1);
    idx_t c = n - prev_cum;          /* column index in row r */

    /* compute factorial of r */
    seq_t fact = factorial(r);

    /* compute Eulerian number E(r+1, c) */
    seq_t euler = eulerian(r + 1, c);

    /* t(r,c) = fact * euler - fact + 1 */
    seq_t result = fact * euler;
    result -= fact;
    result += seq_one();

    return result;
}
