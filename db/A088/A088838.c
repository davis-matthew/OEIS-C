/*
 * A088838  Numerator of the quotient sigma(3n)/sigma(n).
 * https://oeis.org/A088838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac, look
 * Formula:   a(n) = (3^(2+A007949(n))-1)/2 where A007949(n) is the exponent
 *            of the highest power of 3 that divides n
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t
a(idx_t n)
{
    if (n == 0)   /* offset 1, but guard against n==0 */
        return 0;

    /* Find exponent of 3 in n */
    idx_t tmp = n;
    unsigned e = 0;
    while (tmp % 3 == 0) {
        tmp /= 3;
        ++e;
    }

    /* Compute 3^(2+e) */
    seq_t pow = 1;
    const seq_t three = 3;
    for (unsigned i = 0; i < 2 + e; ++i)
        pow *= three;

    /* (pow - 1) is even; divide by 2 */
    return (pow - 1) / 2;
}
