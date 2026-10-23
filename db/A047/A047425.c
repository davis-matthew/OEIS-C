/*
 * A047425  Numbers that are congruent to {3, 4, 5, 6} mod 8.
 * https://oeis.org/A047425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + ((n-1) mod 4) + 3
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047425: Numbers congruent to {3,4,5,6} modulo 8, offset 1 */
seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller */
    idx_t n_minus1 = n - 1;
    idx_t q = n_minus1 / 4;   /* floor((n-1)/4) */
    idx_t r = n_minus1 % 4;   /* (n-1) mod 4 */

    seq_t result = (seq_t)8 * (seq_t)q + (seq_t)r + (seq_t)3;
    return result;
}
