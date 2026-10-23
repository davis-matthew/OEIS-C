/*
 * A028112  Expansion of 1/((1-4x)(1-5x)(1-6x)(1-10x)).
 * https://oeis.org/A028112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^(n+3)-15*6^(n+3)+24*5^(n+3)-10*4^(n+3))/120
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = 1;
    seq_t a1 = 25;
    seq_t a2 = 401;
    seq_t a3 = 5285;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    seq_t an;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = a3 * 25 + a1 * 860;   /* a(n-1)*25 + a(n-3)*860 */
        seq_t term2 = a2 * 224 + a0 * 1200; /* a(n-2)*224 + a(n-4)*1200 */
        an   = term1 - term2;
        a0   = a1;
        a1   = a2;
        a2   = a3;
        a3   = an;
    }
    return a3;
}
