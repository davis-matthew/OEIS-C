/*
 * A081347  First column in maze arrangement of natural numbers.
 * https://oeis.org/A081347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((1 + 2*n^2) + (1 - 2*n)*(-1)^n)/2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    if (n & 1U)   /* odd n */
        return n2 + ns;
    else          /* even n */
        return n2 - ns + (seq_t)1;
}
