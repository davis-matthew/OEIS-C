/*
 * A156718  Numbers k such that k^2 == -1 (mod 13^2).
 * https://oeis.org/A156718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 169*((n+1)/2 - 1) + 70 if n is odd a(n) = 169*((n)/2 - 1)
 *            + 99 if n is even (This generates all positive integers k such
 *            that k^2 ≡ -1 (mod 169), i.e. the numbers of the form 169k ±
 *            70.)
 * Range:     u8 n <= 3, u16 n <= 776, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1, but graceful handling if n == 0 */
    if (n == 0u) {
        return 0;
    }
    if ((n & 1u) != 0u) {          /* odd n */
        idx_t m = (n + 1u) / 2u;
        return (seq_t)169 * ((seq_t)(m - 1u)) + (seq_t)70;
    } else {                       /* even n */
        idx_t m = n / 2u;
        return (seq_t)169 * ((seq_t)(m - 1u)) + (seq_t)99;
    }
}
