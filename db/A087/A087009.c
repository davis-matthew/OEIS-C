/*
 * A087009  Least m such that omega(m) + Omega(m) = n, or 0 if no such m
 *          exists.
 * https://oeis.org/A087009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) * 3 for n > 3 * a(0) = 1, a(1) = 0, a(2) = 2,
 *            a(3) = 4 * (direct computation without a lookup table)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* for n >= 4: a(n) = 3 * 2^(n-3) */
    seq_t result = (seq_t)3;
    /* multiply by 2 (n-3) times */
    for (idx_t i = 4; i <= n; ++i) {
        result *= (seq_t)2;
    }
    return result;
}
