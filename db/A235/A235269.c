/*
 * A235269  a(n) = floor(s*t/(s+t)), where s(n) are the squares, t(n) the
 *          triangular numbers.
 * https://oeis.org/A235269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + n^2) / (3*n + 1))
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; n == 0 is never queried */
    seq_t un = (seq_t)n;
    seq_t num = un * un * un + un * un;  /* n^3 + n^2 */
    seq_t den = 3 * un + 1;              /* 3*n + 1 */
    return num / den;                    /* integer division => floor for positives */
}
