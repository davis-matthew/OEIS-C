/*
 * A143901  Rectangular array R by antidiagonals: R(m,n) = floor((m*n+1)/2).
 * https://oeis.org/A143901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = R(m,n) where R(m,n) = floor((m*n+1)/2); the index n is obtained
   by enumerating the rectangle R(m,n) along its antidiagonals (m+n = constant). */
seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n==0 is undefined – return 0 to avoid
       unsigned underflow. */
    if (n == 0) return 0;

    idx_t prev = 0;          /* number of entries before the current diagonal */
    idx_t D    = 2;          /* current diagonal number (sum m+n) */

    /* Find the first diagonal D such that prev + (D-1) >= n,
       i.e., the diagonal that contains the n‑th term. */
    while (prev + (D - 1) < n) {
        prev += (D - 1);
        ++D;
    }

    /* Position within the diagonal (0‑based). */
    idx_t k = n - prev - 1;

    /* Convert position to (m, l) coordinates.  In diagonal D, we have
       m = k + 1   and   l = D - m.  */
    idx_t m = k + 1;
    idx_t l = D - m;

    /* Compute R(m,l) = floor((m*l+1)/2).  All arithmetic uses seq_t. */
    seq_t prod = (seq_t)m * (seq_t)l;
    return (prod + 1) / 2;
}
