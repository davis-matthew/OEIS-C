/*
 * A155868  Triangle T(n, k) = (-1)^n*StirlingS1(n, j)*StirlingS1(n, n-j),
 *          with T(n, 0) = T(n, n) = 1, read by rows.
 * https://oeis.org/A155868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) = S(r,c) * S(r,r-c) where r and c are the row and
 *            column of the flattened triangle and S(r,·) are unsigned
 *            Stirling numbers of the first kind. Row r starts at index
 *            r*(r+1)/2 in the flattened sequence
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 57, u64 n <= 125,
 *            u128 n <= 334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **St1 = NULL;     /* table of Stirling numbers */
static idx_t  St1_max = 0;     /* highest row computed */

/* Ensure rows 0..max_row of the Stirling table are available. */
static void ensure_stirling(idx_t max_row)
{
    if (St1_max >= max_row)
        return;

    /* Allocate or grow the row-pointer table. */
    if (St1 == NULL) {
        St1 = (seq_t **)malloc((size_t)(max_row + 1) * sizeof(seq_t *));
        for (idx_t i = 0; i <= max_row; ++i)
            St1[i] = NULL;
    } else if (max_row > St1_max) {
        St1 = (seq_t **)realloc(St1, (size_t)(max_row + 1) * sizeof(seq_t *));
        for (idx_t i = St1_max + 1; i <= max_row; ++i)
            St1[i] = NULL;
    }

    /* Initialise row 0 if not done yet. */
    if (St1[0] == NULL) {
        St1[0] = (seq_t *)malloc(sizeof(seq_t));
        St1[0][0] = 1;
    }

    /* Compute missing rows. */
    for (idx_t n = St1_max + 1; n <= max_row; ++n) {
        St1[n] = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        St1[n][0] = 0;                     /* S(n,0) = 0 for n>0 */
        for (idx_t k = 1; k <= n; ++k) {
            seq_t a = St1[n-1][k-1];
            seq_t b = (k <= n-1) ? St1[n-1][k] : 0;
            seq_t mul = b * (seq_t)(n-1);   /* (n-1) * S(n-1,k) */
            St1[n][k] = a + mul;
        }
    }
    St1_max = max_row;
}

/* Return the nth term of A155868, 0‑based indexing. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column index c within that row. */
    idx_t c = n - (r * (r + 1)) / 2;

    /* Boundary elements are defined to be 1. */
    if (c == 0 || c == r)
        return (seq_t)1;

    /* Make sure needed Stirling numbers are available. */
    ensure_stirling(r);

    seq_t u1 = St1[r][c];
    seq_t u2 = St1[r][r - c];

    return u1 * u2;
}
