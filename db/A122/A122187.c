/*
 * A122187  First row sum of the matrix M^n, where M is the 3 X 3 matrix [[6,
 *          5, 3], [5, 4, 2], [3, 2, 1]] (n>=0).
 * https://oeis.org/A122187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1)+4*a(n-2)-a(n-3), a(1)=1, a(2)=14, a(3)=157
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */
    if (n == 1) return 1;
    if (n == 2) return 14;
    if (n == 3) return 157;

    seq_t a1 = 1;       /* a(1) */
    seq_t a2 = 14;      /* a(2) */
    seq_t a3 = 157;     /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = a3 * 11;   /* 11*a(i-1) */
        next += a2 * 4;   /* + 4*a(i-2) */
        next -= a1;       /* - a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
