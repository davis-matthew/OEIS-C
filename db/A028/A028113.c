/*
 * A028113  Expansion of 1/((1-4x)(1-5x)(1-6x)(1-11x)).
 * https://oeis.org/A028113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^(n+3)-21*6^(n+3)+7*5^(n+4)-15*4^(n+3))/210
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)437;
    if (n == 3) return (seq_t)6082;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)26;
    seq_t a2 = (seq_t)437;
    seq_t a3 = (seq_t)6082;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)26 * a3;
        next -= (seq_t)239 * a2;
        next += (seq_t)934 * a1;
        next -= (seq_t)1320 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
