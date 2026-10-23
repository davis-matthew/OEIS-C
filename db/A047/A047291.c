/*
 * A047291  Numbers that are congruent to {0, 1, 4, 6} mod 7.
 * https://oeis.org/A047291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/4) + [0,1,4,6][(n-1) mod 4]
 * Range:     u8 n <= 146, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates the integers congruent to 0,1,4 or 6 modulo 7. */

seq_t a(idx_t n)
{
    /* n is 1-based; sequence is defined for n >= 1. */
    idx_t n1 = n - 1;                 /* safe because n >= 1 */
    seq_t q   = (seq_t)n1 / 4;        /* block index */
    idx_t k   = n1 % 4;               /* position inside the block */

    static const seq_t offs[4] = {0, 1, 4, 6};
    return (seq_t)7 * q + offs[k];
}
