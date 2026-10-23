/*
 * A010179  Continued fraction for sqrt(114).
 * https://oeis.org/A010179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=10. For n≥1: a(n) is 2 if 2 divides n but 3 does not, 10 if
 *            3 divides n but 2 does not, 20 if both 2 and 3 divide n, 1
 *            otherwise. This implements the multiplicative rule a(2^e)=2,
 *            a(3^e)=10, a(p^e)=1 for p≥5
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)10;
    seq_t res = (seq_t)1;
    if ((n % 2) == 0)
        res *= (seq_t)2;
    if ((n % 3) == 0)
        res *= (seq_t)10;
    return res;
}
