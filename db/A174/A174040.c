/*
 * A174040  A triangle sequence of the
 *          form:q=3:t(n,m,q)=Sum[q^i*Floor[Eulerian[n + 1, m]/2^i], {i, 0,
 *          10}].
 * https://oeis.org/A174040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = Sum_{i=0..10} 3^i * floor( Eulerian[r+1, m] / 2^i ) where
 *            r = most 0‑based row with r(r+1)/2 ≤ n m = n – r(r+1)/2 (column
 *            index in the row)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the A174040 term for a given index n. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row.  m ≤ r. */
    idx_t m = n - r * (r + 1) / 2;

    /* We need Eulerian numbers up to n = r+1. */
    idx_t target_n = r + 1;               /* target Eulerian n = r+1  */
    size_t sz = (size_t)target_n + 1;     /* array length needed */

    /* Allocate arrays for two consecutive Eulerian rows. */
    seq_t *e  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *ne = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!e || !ne) {                      /* allocation failed */
        return 0;
    }

    /* Base row: Eulerian(1,0) = 1 */
    e[0] = 1;

    /* Build Eulerian rows iteratively up to target_n. */
    for (idx_t k = 2; k <= target_n; ++k) {
        for (idx_t j = 0; j < k; ++j) {
            seq_t a_val = (j >= 1) ? e[j - 1] : 0;
            seq_t b_val = (j <= k - 2) ? e[j] : 0;
            ne[j] = (seq_t)(k - j) * a_val + (seq_t)(j + 1) * b_val;
        }
        /* Copy new row into the current row buffer. */
        for (idx_t j = 0; j < k; ++j)
            e[j] = ne[j];
    }

    /* Eulerian value for the desired position (target_n, m). */
    seq_t e_val = e[m];

    /* Free temporary storage. */
    free(e);
    free(ne);

    /* Pre‑compute powers of 3 up to 3^10 once. */
    static seq_t pow3[11];
    static bool pow3_done = false;
    if (!pow3_done) {
        pow3[0] = 1;
        for (int i = 1; i < 11; ++i)
            pow3[i] = pow3[i - 1] * (seq_t)3;
        pow3_done = true;
    }

    /* Sum the weighted floor divisions. */
    seq_t sum = 0;
    for (int i = 0; i < 11; ++i) {
        seq_t floor_div = e_val >> i;          /* floor(e_val / 2^i) */
        sum += floor_div * pow3[i];
    }

    return sum;
}
