/*
 * A000038  Twice A000007.
 * https://oeis.org/A000038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = 2*A000007(n), i.e. a(0) = 2, a(n) = 0 for n >= 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    if (n == 0)
        r = 2;
    return r;
}
