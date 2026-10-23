/*
 * A047329  Numbers that are congruent to {1, 3, 5, 6} mod 7.
 * https://oeis.org/A047329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7n-1)/4)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A047329: numbers congruent to {1,3,5,6} mod 7. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                               /* Undefined for n < 1, but guard anyway. */

    idx_t q_idx = n / 4;                       /* Integer division of the index.   */
    idx_t r_idx = n % 4;                       /* Remainder 0…3.                 */

    seq_t q = (seq_t)q_idx;                    /* Convert to seq_t for arithmetic  */
    seq_t r = (seq_t)r_idx;

    /* Compute floor((7n-1)/4) without overflow in seq_t.                */
    seq_t res = q * 7 + ((7 * r) / 4);         /* 7*n/4 part: 7q + floor(7r/4)   */
    if (r == 0)
        res -= 1;                              /* Subtract 1 when r == 0.         */

    return res;
}
