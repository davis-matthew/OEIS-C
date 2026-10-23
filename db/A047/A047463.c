/*
 * A047463  Numbers that are congruent to {2, 4} mod 8.
 * https://oeis.org/A047463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - (-1)^n - 3
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence has offset 1; a(1) is the first term. */
    if (n == 0) return 0;

    seq_t N = (seq_t)n;
    if ((n & 1) == 0)          /* n is even: (-1)^n = 1  */
        return 4 * N - 4;      /* a = 4n - 1 - 3 = 4(n-1)   */
    else                        /* n is odd:  (-1)^n = -1 */
        return 4 * N - 2;      /* a = 4n + 1 - 3 = 4n - 2   */
}
