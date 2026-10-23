/*
 * A123726  Denominators of fractional partial quotients appearing in a
 *          continued fraction for the power series Sum_{n>=0} x^(2^n -
 *          1)/(n+1)^s.
 * https://oeis.org/A123726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, mult
 * Formula:   a(0)=1; for n>=1, a(n) = (v2(n)+1)^2, where v2(n) is the
 *            exponent of the highest power of 2 dividing n
 * Range:     u8 n <= 9722, u16 n <= 9722, u32 n <= 9722, u64 n <= 9722,
 *            u128 n <= 9722, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t m = n;
    unsigned int e = 0;
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;
        e++;
    }
    seq_t val = (seq_t)(e + 1) * (seq_t)(e + 1);
    return val;
}
