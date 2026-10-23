/*
 * A047278  Numbers that are congruent to {1, 2, 6} mod 7.
 * https://oeis.org/A047278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 7k-1, a(3k-1) = 7k-5, a(3k-2) = 7k-6
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; undefined for 0 */
        return (seq_t)0;

    idx_t rem = n % 3;
    if (rem == 1) {      /* n = 3k-2  */
        idx_t k = (n + 2) / 3;
        return (seq_t)7 * (seq_t)k - (seq_t)6;
    } else if (rem == 2) { /* n = 3k-1 */
        idx_t k = (n + 1) / 3;
        return (seq_t)7 * (seq_t)k - (seq_t)5;
    } else {              /* n = 3k  */
        idx_t k = n / 3;
        return (seq_t)7 * (seq_t)k - (seq_t)1;
    }
}
