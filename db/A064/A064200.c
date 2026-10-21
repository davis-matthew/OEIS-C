/*
 * A064200  a(n) = 12*n*(n-1).
 * https://oeis.org/A064200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n*(n-1)
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn <= 1)                /* n = 0 or 1 → 0 */
        return (seq_t)0;
    seq_t prod = nn * (nn - 1); /* n*(n-1) */
    return prod * (seq_t)12;    /* ×12 */
}
