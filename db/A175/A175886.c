/*
 * A175886  Numbers that are congruent to {1, 12} mod 13.
 * https://oeis.org/A175886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (26*n + 9*(-1)^n - 13)/4
 *  (for n >= 1)
 */
seq_t a(idx_t n)
{
    /* Use 128-bit arithmetic to avoid overflow on large indices */
    unsigned __int128 num = (unsigned __int128)26 * n;

    /* (+9) for even n, (-9) for odd n */
    if ((n & 1) == 0)          /* even */
        num += 9;
    else                       /* odd */
        num -= 9;

    num -= 13;                 /* subtract the constant term */

    return (seq_t)(num / 4);   /* final integer division */
}
