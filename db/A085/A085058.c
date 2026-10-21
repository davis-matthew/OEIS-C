/*
 * A085058  a(n) = A001511(n+1) + 1.
 * https://oeis.org/A085058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (2n+2) that are powers of two =
 *            v2(n+1) + 2, where v2(x) is the exponent of 2 in the
 *            factorisation of x
 * Range:     u8 n <= 9639, u16 n <= 9639, u32 n <= 9639, u64 n <= 9639,
 *            u128 n <= 9639, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute v2(n+1) by counting trailing zeros of n+1. */
    idx_t m = n + 1;          /* m >= 1 for all practical n */
    seq_t cnt = 0;
    while ((m & 1) == 0)
    {
        cnt++;
        m >>= 1;
    }
    return cnt + 2;
}
