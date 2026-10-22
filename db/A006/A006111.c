/*
 * A006111  Gaussian binomial coefficient [ n,2 ] for q=5.
 * https://oeis.org/A006111
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x^2/((1-x)(1-5x)(1-25x)); computed as d(n) = 5*d(n-1) + 1,
 *            a(n) = 25*a(n-1) + d(n), with d(2) = 1, a(2) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t d = 1;
    seq_t f = 1;
    idx_t i;
    for (i = 3; i <= n; i++) {
        d = d * 5 + 1;
        f = f * 25 + d;
    }
    return f;
}
