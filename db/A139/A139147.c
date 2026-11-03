/*
 * A139147  Triangle read by rows: replace A003983(n,k) with F(n).
 * https://oeis.org/A139147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(min(k, n-k)+1) where k is the 0‑based *
 *            position inside the triangle row that contains the n‑th term. *
 *            The row index r (starting at 0) is found by subtracting row
 *            lengths * until the remaining count fits in a row; then k =
 *            remaining-1. * The triangle T(r,k) defined in the OEIS e...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t fem_idx(idx_t m)
{
    /* Return the m-th Fibonacci number (F(1)=F(2)=1). */
    if (m <= 2) return (seq_t)1;
    seq_t a = 1, b = 1;
    for (idx_t i = 3; i <= m; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index into the flattened triangle. */
    idx_t remaining = n;   /* terms left to process */
    idx_t row = 0;         /* current row index, starting at 0 */

    /* Find the row that contains the n-th term. */
    while (remaining > row + 1) {
        remaining -= (row + 1);
        ++row;
    }

    /* remaining is now the 1‑based position within this row (1..row+1). */
    idx_t k = remaining - 1;          /* convert to 0‑based column index */
    idx_t d = (k <= row - k) ? k : (row - k);
    idx_t t = d + 1;                  /* Fibonacci index to use */

    return fem_idx(t);
}
