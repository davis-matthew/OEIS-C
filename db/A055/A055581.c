/*
 * A055581  Fifth column of triangle A055252.
 * https://oeis.org/A055581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2-n+4)2^(n+1)-7-n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 51, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t n_sq = n_s * n_s;
    seq_t term = n_sq - n_s + (seq_t)4;          /* (n^2 - n + 4) */

    idx_t exp = n + (idx_t)1;
    seq_t power = (seq_t)1;                     /* 2^(n+1) */
    for (idx_t i = (idx_t)0; i < exp; ++i)
        power *= (seq_t)2;

    seq_t result = term * power;                /* (n^2-n+4) * 2^(n+1) */
    seq_t sub     = (seq_t)(n + (idx_t)7);       /* 7 + n */
    result -= sub;

    return result;
}
