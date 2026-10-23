/*
 * A047546  Numbers that are congruent to {2, 3, 4, 7} mod 8.
 * https://oeis.org/A047546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {2,3,4,7}[ (n-1) mod 4 ]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers congruent to 2,3,4,7 (mod 8). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* value for n<1 is irrelevant */
    static const seq_t offs[4] = { (seq_t)2,
                                   (seq_t)3,
                                   (seq_t)4,
                                   (seq_t)7 };
    idx_t m      = n - 1;                  /* zero‑based index */
    idx_t block  = m >> 2;                  /* floor((n-1)/4) */
    idx_t t      = m & 3;                   /* (n-1) mod 4 */
    return offs[t] + (seq_t)block * (seq_t)8;
}
