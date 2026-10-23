/*
 * A011869  a(n) = floor(n*(n-1)/16).
 * https://oeis.org/A011869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/16)
 * Range:     u8 n <= 64, u16 n <= 1024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n=0 or 1 the product underflows, but the
       mathematical value is 0, so handle them separately. */
    if (n < 2) return 0;

    seq_t nn  = (seq_t)n;      /* cast to seq_t just to be safe */
    seq_t nm1 = (seq_t)(n - 1);
    return nn * nm1 / (seq_t)16;
}
