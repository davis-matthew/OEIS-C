/*
 * A047395  Numbers that are congruent to {0, 2, 6} mod 8.
 * https://oeis.org/A047395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ( n - 1 + floor(n/3) ) (offset 1)
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1. If n == 0, return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    seq_t mn = (seq_t)n;              /* cast index to seq_t          */
    seq_t q  = (seq_t)(n / 3);        /* integer division gives floor */
    return 2 * (mn - 1 + q);
}
