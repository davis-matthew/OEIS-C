/*
 * A193449  Products of the Jacobsthal numbers and the integers: a(n) = n *
 *          A001045(n+1).
 * https://oeis.org/A193449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2^(n + 1) + (-1)^n)/3
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t two = ((seq_t)1 << (n + 1));

    /* numerator: 2^(n+1) + (-1)^n  (even n: +1, odd n: -1) */
    seq_t num = (n & 1) ? (two - 1) : (two + 1);

    /* divide first to keep numbers small */
    seq_t q = num / 3;

    /* final value */
    return q * (seq_t)n;
}
