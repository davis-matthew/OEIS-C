/*
 * A050292  a(2*n) = 2*n - a(n), a(2*n+1) = 2*n + 1 - a(n) (for n >= 0).
 * https://oeis.org/A050292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n - a(floor(n/2)), a(0) = 0
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A050292 */

seq_t a(idx_t n)
{
    if (n == 0)            /* base case: a(0) = 0 */
        return 0;

    /* Recursive step: a(n) = n - a(floor(n/2)) */
    seq_t sub = a(n >> 1); /* floor(n/2) is n>>1 for unsigned n */
    return (seq_t)n - sub;
}
