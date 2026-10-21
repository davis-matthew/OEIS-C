/*
 * A064335  a(n) = 6*(2*n)!/(n+2).
 * https://oeis.org/A064335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*(2*n)!/(n+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t i, limit = 2 * n;
    for (i = 1; i <= limit; ++i)
        prod *= (seq_t)i;          /* prod = (2*n)!          */
    prod *= 6;                     /* prod = 6*(2*n)!       */
    prod /= (seq_t)(n + 2);        /* prod = 6*(2*n)!/(n+2) */
    return prod;
}
