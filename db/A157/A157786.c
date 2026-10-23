/*
 * A157786  a(n) = 27225*n^2 - 15248*n + 2135.
 * https://oeis.org/A157786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27225*n^2 - 15248*n + 2135
 * Range:     u8 none, u16 n <= 1, u32 n <= 397, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* cast index to seq_t width */
    unsigned long long t = 27225ull * (unsigned long long)nn;
    t = t * (unsigned long long)nn;     /* 27225*n^2                */
    t = t - 15248ull * (unsigned long long)nn + 2135ull; /* -15248*n + 2135 */
    return (seq_t)t;
}
