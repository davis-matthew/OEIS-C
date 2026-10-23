/*
 * A047582  Numbers that are congruent to {3, 5, 6, 7} mod 8.
 * https://oeis.org/A047582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 8*floor((n-1)/4) + {3,5,6,7}[ (n-1) mod 4 ]
 * Sequence A047582: Positive integers congruent to 3,5,6,7 modulo 8.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* Undefined for offset 0, but safe */
    seq_t n_minus1 = (seq_t)(n - 1U);           /* n-1 fits in idx_t, cast to seq_t */
    seq_t block = n_minus1 / 4;                 /* floor((n-1)/4) as seq_t */
    seq_t base  = block << 3;                   /* multiply by 8 using shift */
    const seq_t offsets[4] = {3, 5, 6, 7};
    idx_t r = (n - 1U) % 4;                     /* remainder modulo 4 as idx_t */
    return base + offsets[r];
}
