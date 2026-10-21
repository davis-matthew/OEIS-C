/*
 * A072079  Sum of 3-smooth divisors of n.
 * https://oeis.org/A072079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (2^(v2(n)+1)-1)*(3^(v3(n)+1)-1)/2 * where v2 is the
 *            exponent of 2 dividing n and v3 the exponent of 3 dividing n
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1 */

    /* factor out powers of 2 and 3 */
    idx_t e2 = 0, e3 = 0;
    idx_t t = n;
    while ((t & 1) == 0) { e2++; t >>= 1; }
    while (t % 3 == 0) { e3++; t /= 3; }

    /* 2^(e2+1) - 1 */
    seq_t two_pow = 1;
    for (idx_t i = 0; i < e2 + 1; ++i) two_pow *= (seq_t)2;
    two_pow -= 1;

    /* 3^(e3+1) - 1 */
    seq_t three_pow = 1;
    for (idx_t i = 0; i < e3 + 1; ++i) three_pow *= (seq_t)3;
    three_pow -= 1;

    /* final result */
    return (two_pow * three_pow) / 2;
}
