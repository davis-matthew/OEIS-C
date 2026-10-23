/*
 * A168282  a(n) = (10*n + 5*(-1)^n - 1)/4.
 * https://oeis.org/A168282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*floor(n/2) + 1
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (derived from a(n) = (10*n + 5*(-1)^n - 1)/4) */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first to avoid overflow on large idx_t */
    seq_t k = (seq_t)n / 2;   /* floor(n/2) */
    return k * 5 + 1;
}
