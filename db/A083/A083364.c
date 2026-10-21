/*
 * A083364  Antidiagonal sums of table A083362.
 * https://oeis.org/A083364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 12*n^2 + 18*n + 9 + (2*n^2 + 2*n - 1)*(-1)^n) /
 *            8
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for intermediate values */
    __int128 nn = (__int128)(unsigned long long)n;
    __int128 num = 4*nn*nn*nn + 12*nn*nn + 18*nn + 9;
    __int128 part = 2*nn*nn + 2*nn - 1;   /* may be negative for n = 0 or 1 */

    if (n & 1ULL)          /* n odd: subtract part */
        num -= part;
    else                   /* n even: add part */
        num += part;

    return (seq_t)(num / 8);
}
