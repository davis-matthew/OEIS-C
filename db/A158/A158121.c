/*
 * A158121  Given n points in the complex plane, let M(n) the number of
 *          distinct Moebius transformations that take 3 distinct points to 3
 *          distinct points. Note that the triples may have some or all of the
 *          points in common.
 * https://oeis.org/A158121
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*C(n,3)^2 - C(n,3) + 1, for n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;               /* undefined for n < 3 */

    /* Compute C(n,3) = n*(n-1)*(n-2)/6 safely in seq_t */
    seq_t cn = (seq_t)n;
    seq_t comb3 = cn * (cn - 1) * (cn - 2) / 6;

    /* Compute 6*comb3^2 - comb3 + 1 */
    seq_t t = comb3 * comb3;     /* comb3² */
    t = t * 6 - comb3 + 1;       /* 6*comb3² - comb3 + 1 */

    return t;
}
