/*
 * A168195  a(n) = 2*n - a(n-1) + 1 with n>1, a(1)=5.
 * https://oeis.org/A168195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 - 3*(-1)^n Ranges: a(1) = 5, a(2) = 0, a(3) = 7,
 *            ... The formula holds for all n ≥ 1
 * Range:     u8 n <= 252, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* idx_t is unsigned.  For odd n, a(n) = n + 4.  For even n, a(n) = n - 2.  */
    if (n & 1)       /* odd */
        return (seq_t)n + 4;
    else             /* even */
        return (seq_t)n - 2;
}
