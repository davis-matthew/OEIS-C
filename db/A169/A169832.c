/*
 * A169832  a(n) = 15*2^(n+1) - (5*n^2+22*n+30).
 * https://oeis.org/A169832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*2^(n+1) - (5*n^2+22*n+30)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i <= n; ++i)
    {
        pow2 *= (seq_t)2;
    }

    /* convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* compute the quadratic part: 5*n^2 + 22*n + 30 */
    seq_t quad = (seq_t)5 * N * N + (seq_t)22 * N + (seq_t)30;

    /* final result */
    return (seq_t)15 * pow2 - quad;
}
