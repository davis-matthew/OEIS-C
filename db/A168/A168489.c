/*
 * A168489  Numbers that are congruent to {7,11} mod 12.
 * https://oeis.org/A168489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n - (-1)^n (with a(1)=7)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n >= 1 – this is a safe fallback */
    if (n & 1)                     /* n is odd:  (-1)^n = -1  →  a = 6*n + 1 */
        return (seq_t)6 * n + 1;
    else                           /* n is even: (-1)^n =  1  →  a = 6*n - 1 */
        return (seq_t)6 * n - 1;
}
