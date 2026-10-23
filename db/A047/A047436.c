/*
 * A047436  Numbers that are congruent to {5, 6} mod 8.
 * https://oeis.org/A047436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*((n-1)/2)+5 if n is odd, otherwise 8*(n/2)-2
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, return 0 */

    if (n & 1U) {              /* odd n: a(2k+1) = 8k + 5 */
        return (seq_t)(8 * ((n - 1) / 2) + 5);
    } else {                   /* even n: a(2k) = 8k - 2 */
        return (seq_t)(8 * (n / 2) - 2);
    }
}
