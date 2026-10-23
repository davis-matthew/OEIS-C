/*
 * A168419  a(n) = 9*floor(n/2).
 * https://oeis.org/A168419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*floor(n/2)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Offset: 1, so a(1) is the first term listed) */
seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid overflow during multiplication.
       Then compute floor(n/2) using unsigned integer division. */
    seq_t rn = (seq_t)n;
    return (seq_t)9 * (rn / 2);
}
