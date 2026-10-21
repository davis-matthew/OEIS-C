/*
 * A055831  T(n,n-4), where T is the array in A055830.
 * https://oeis.org/A055831
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n^2+6*n-10)/6, n>3
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)                     /* offset starts at 4, below that undefined */
        return 0;

    seq_t ns = (seq_t)n;           /* convert index to seq_t type */

    /* Compute (n-3) * (n^2 + 6*n - 10) / 6 */
    seq_t term1 = ns - 3;
    seq_t term2 = ns * ns + 6 * ns - 10;
    seq_t prod  = term1 * term2;
    return prod / 6;
}
