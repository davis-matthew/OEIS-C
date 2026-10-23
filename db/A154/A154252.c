/*
 * A154252  Expansion of (1-x+8x^2)/((1-x)(1-2x)) .
 * https://oeis.org/A154252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 2*a(n-1) + 8 for n >= 2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t res = (seq_t)2;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) /* compute a(2) … a(n) */
        res = (res << 1) + 8;      /* res = 2*res + 8 */

    return res;
}
