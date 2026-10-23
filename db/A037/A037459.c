/*
 * A037459  Sum{d(i)*5^i: i=0,1,...,m}, where Sum{d(i)*4^i: i=0,1,...,m} is
 *          the base 4 representation of n.
 * https://oeis.org/A037459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥0} d_i * 5^i, where d_i are the base‑4 digits of n
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow5   = 1;

    while (n > 0)
    {
        unsigned d = (unsigned)(n & 3U);  // equivalent to n % 4
        result += (seq_t)d * pow5;
        n >>= 2;                         // equivalent to n /= 4
        pow5 *= 5;
    }

    return result;
}
