/*
 * A080278  a(n) = (3^(v_3(n) + 1) - 1)/2, where v_3(n) = highest power of 3
 *          dividing n = A007949(n).
 * https://oeis.org/A080278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, frac
 * Formula:   a(n) = (3^(v_3(n)+1)-1)/2 where v_3(n) is the exponent of 3
 *            dividing n (OEIS A080278)
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute v_3(n) */
    idx_t nn = n;
    idx_t e  = 0;
    while ((nn % (idx_t)3) == 0)
    {
        ++e;
        nn /= (idx_t)3;
    }

    /* compute 3^(e+1) in seq_t */
    seq_t pow   = (seq_t)1;
    seq_t three = (seq_t)3;
    for (idx_t i = 0; i < e + 1; ++i)
        pow = pow * three;

    /* (3^(e+1)-1)/2 */
    seq_t val = (pow - (seq_t)1) / (seq_t)2;
    return val;
}
