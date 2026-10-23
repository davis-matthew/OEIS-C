/*
 * A028899  Map n = Sum c_i 10^i to a(n) = Sum c_i 4^i.
 * https://oeis.org/A028899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Σ_{i=0}^{∞} (floor(n/10^i) mod 10) * 4^i (i.e., interpret
 *            the decimal digits of n as digits in base 4)
 * Range:     u8 n <= 2667, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;       /* 4^i carrier */
    idx_t m = n;            /* local copy */

    while (m > 0) {
        result += (seq_t)(m % 10) * power;
        m /= 10;
        power *= 4;
    }
    return result;
}
