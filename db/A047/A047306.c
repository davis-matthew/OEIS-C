/*
 * A047306  Numbers that are congruent to {0, 2, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A047306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-2)/6) (for n ≥ 1 with a(1)=0)
 * Range:     u8 n <= 219, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handles the special case n == 1, where n-2 would underflow */
    if (n < 2u) {
        return (seq_t)0;
    }
    /* For n ≥ 2, the unsigned subtraction is safe */
    return (seq_t)n + (seq_t)((n - 2) / 6u);
}
