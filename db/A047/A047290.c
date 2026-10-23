/*
 * A047290  Numbers that are congruent to {1, 4, 6} mod 7.
 * https://oeis.org/A047290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(3k) = 7k-1, a(3k-1) = 7k-3, a(3k-2) = 7k-6 (OEIS
 *            A047290)
 * Range:     u8 n <= 109, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) {                     /* n = 3k  */
        idx_t k = n / 3;
        return (seq_t)7 * (seq_t)k - 1;
    } else if (r == 1) {              /* n = 3k–1 */
        idx_t k = (n + 2) / 3;
        return (seq_t)7 * (seq_t)k - 6;
    } else {                          /* n = 3k–2 */
        idx_t k = (n + 1) / 3;
        return (seq_t)7 * (seq_t)k - 3;
    }
}
