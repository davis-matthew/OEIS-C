/*
 * A152949  a(n) = 3 + binomial(n-1,2).
 * https://oeis.org/A152949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 3*n + 8) / 2
 * Range:     u8 n <= 17, u16 n <= 304, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for safe arithmetic */
    seq_t sn = (seq_t)n;
    /* Compute n^2 - 3*n + 8 */
    seq_t t = sn * sn - sn * (seq_t)3 + (seq_t)8;
    /* Divide by 2 (truncating integer division) */
    return t / (seq_t)2;
}
