/*
 * A095799  Bell triangle A011971 squared.
 * https://oeis.org/A095799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=col}^{row} M[row][k] * M[k][col]
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 48, u64 n <= 121,
 *            u128 n <= 329, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           where M is the Bell triangle A011971 and (row,col)   */
/*           come from the triangular indexing of n.            */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for the Bell triangle rows */
static seq_t **Bell = NULL;
static idx_t Bell_max = 0;

/* Ensure the Bell triangle has at least row r (1‑based) computed */
static void ensure_bell_row(idx_t r)
{
    if (r <= Bell_max) return;
    Bell = (seq_t**)realloc(Bell, r * sizeof(seq_t*));
    for (idx_t i = Bell_max + 1; i <= r; ++i) {
        Bell[i-1] = (seq_t*)malloc(sizeof(seq_t) * i);
        if (i == 1) {
            /* First row */
            Bell[0][0] = (seq_t)1;
        } else {
            /* First element of row i is last element of previous row */
            Bell[i-1][0] = Bell[i-2][i-2];
            /* Remaining elements: sum of left neighbor and upper-left element */
            for (idx_t j = 1; j < i; ++j) {
                Bell[i-1][j] = Bell[i-1][j-1] + Bell[i-2][j-1];
            }
        }
    }
    Bell_max = r;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range – not used in tests */

    /* Find the row r (1‑based) such that (r−1)r/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index col (1‑based) within that row */
    idx_t col = n - ((r - 1) * r) / 2;

    /* Compute the Bell triangle up to the required row if not yet cached */
    ensure_bell_row(r);

    /* Compute the squared entry: sum_{k=col}^{r} M[row][k] * M[k][col] */
    seq_t sum = (seq_t)0;
    for (idx_t k = col; k <= r; ++k) {
        sum += Bell[r-1][k-1] * Bell[k-1][col-1];
    }
    return sum;
}
