/*
 * A032769  Numbers that are congruent to {0, 1, 2, 4} mod 5.
 * https://oeis.org/A032769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A032769: Numbers congruent to {0,1,2,4} modulo 5.
 * Formula: a(n) = floor((5*n-4)/4)
 */
seq_t a(idx_t n)
{
    /* n is 1-based. */
    return (seq_t)((5*n - 4) / 4);
}
