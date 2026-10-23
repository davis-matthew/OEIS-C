/*
 * A168672  Numbers that are congruent to {2,13} mod 17.
 * https://oeis.org/A168672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (34*n + 5*(-1)^n - 21)/4
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the numerator using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 num = (unsigned __int128)34 * (unsigned __int128)n;
    if ((n & 1) == 0)           /* even n: (-1)^n = 1  */
        num -= 16;              /* 34*n + 5 - 21 = 34*n - 16 */
    else                        /* odd n:  (-1)^n = -1 */
        num -= 26;              /* 34*n - 5 - 21 = 34*n - 26 */
    return (seq_t)(num >> 2);   /* divide by 4 */
}
