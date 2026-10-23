/*
 * A106387  Numbers j such that 6j^2 + 6j + 1 = 11k.
 * https://oeis.org/A106387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * k - 7 for odd n = 2k-1, a(n) = 11 * k - 5 for even n
 *            = 2k. (Equivalently, a(n) = (22*n - 7*(-1)^n - 13)/4.)
 * Range:     u8 n <= 46, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 1, but return 0 for safety */ 
        return 0;

    if (n & 1) {                 /* odd n = 2k-1 */
        seq_t k = ((seq_t)n + 1) / 2;
        return 11 * k - 7;
    } else {                     /* even n = 2k */
        seq_t k = (seq_t)n / 2;
        return 11 * k - 5;
    }
}
