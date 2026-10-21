/*
 * A066481  Largest anti-divisor of n.
 * https://oeis.org/A066481
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n + 1) / 3)
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined, but any return value is acceptable. */
    if (n < 3)
        return (seq_t)0;

    seq_t N = (seq_t)n;
    return (N * 2 + 1) / 3;
}
