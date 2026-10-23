/*
 * A194599  Units' digits of the nonzero hexagonal numbers.
 * https://oeis.org/A194599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n*(2*n-1)) mod 10
 * Range:     u8 n <= 11, u16 n <= 224, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work entirely in seq_t to avoid overflow and keep the
       implementation uniform across all integer sizes.           */
    seq_t tn   = (seq_t)n;          /* cast index to seq_t          */
    seq_t prod = tn * (tn * 2 - 1); /* n * (2n - 1)                 */
    return prod % (seq_t)10;        /* units digit of the hexagonal number */
}
