/*
 * A061317  Split positive integers into extending even groups and sum: 1+2,
 *          3+4+5+6, 7+8+9+10+11+12, 13+14+15+16+17+18+19+20, ...
 * https://oeis.org/A061317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 + n
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t N = (seq_t)n;

    /* Compute n^2, then n^3 */
    seq_t n2 = N * N;      /* n^2 */
    seq_t n3 = n2 * N;     /* n^3 */

    /* Compute 2*n^3 + n */
    seq_t two = (seq_t)2;
    seq_t result = n3 * two + N;
    return result;
}
