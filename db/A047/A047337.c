/*
 * A047337  Numbers that are congruent to {0, 1, 2, 3, 4} mod 7.
 * https://oeis.org/A047337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/5) + ((n-1) mod 5)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the sequence definition */
    idx_t k = n - 1;          /* shift to 0‑based index */
    seq_t quint = (seq_t)(k / 5);   /* quotient of division by 5 */
    seq_t mod    = (seq_t)(k % 5);   /* remainder of division by 5 */
    return 7 * quint + mod;
}
