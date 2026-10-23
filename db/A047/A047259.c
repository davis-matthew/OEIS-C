/*
 * A047259  Numbers that are congruent to {1, 4, 5} mod 6.
 * https://oeis.org/A047259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n if n mod 3 = 2; otherwise a(n) = 2*n - 1 (This
 *            generates the sequence of integers congruent to 1, 4 or 5 modulo
 *            6.)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the problem statement. */
    if (n % 3 == 2)
        return (seq_t)n * 2;          /* 2*n   for n ≡ 2 (mod 3) */
    else
        return (seq_t)n * 2 - 1;      /* 2*n-1 otherwise */
}
