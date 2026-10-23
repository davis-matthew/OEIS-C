/*
 * A133083  A000012 * A133080.
 * https://oeis.org/A133083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 if the column index (in its triangular row) is odd and
 *            not the last column, otherwise 1. Formally, let i = ⌈(√(8n+1) –
 *            1)/2⌉, the row index; let j = n – i(i–1)/2, the 0‑based column
 *            in that row; then a(n) = 2 if j is even (i.e. j+1 is odd) and j
 *            < i–1, else 1
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i = 1;
    idx_t tri = 1;                 /* triangular number T_i = i*(i+1)/2   */
    while (tri < n) {
        ++i;
        tri += i;                  /* update to T_i                     */
    }

    idx_t start = tri - i + 1;      /* first index of row i              */
    idx_t j0 = n - start;          /* 0‑based column in row i           */
    idx_t j1 = j0 + 1;             /* 1‑based column                     */

    seq_t result = 1;
    if ((j1 & 1) && (j1 < i))      /* odd column and not the last one  */
        result = 2;
    return result;
}
