/*
 * A037465  a(n) = Sum_{i=0..m} d(i)*6^i, where Sum_{i=0..m} d(i)*5^i is the
 *          base 5 representation of n.
 * https://oeis.org/A037465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i=0..m} d(i)*6^i, where Sum_{i=0..m} d(i)*5^i is the
 *            base‑5 representation of n
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow6   = 1;
    idx_t temp   = n;

    while (temp > 0)
    {
        idx_t digit = temp % 5;
        result += (seq_t)digit * pow6;
        pow6 *= 6;
        temp  /= 5;
    }
    return result;
}
