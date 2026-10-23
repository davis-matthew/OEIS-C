/*
 * A168465  Numbers that are congruent to 2 or 7 mod 11.
 * https://oeis.org/A168465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n - (-1)^n - 3) / 4 - 3 Equivalent: a(2k) = 11*k - 4
 *            a(2k+1) = 11*k + 2
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* behaviour below offset irrelevant */
    if (n & 1) {                         /* odd indices */
        return (seq_t)11 * ((n - 1) / 2) + 2;
    } else {                             /* even indices */
        return (seq_t)11 * (n / 2) - 4;
    }
}
