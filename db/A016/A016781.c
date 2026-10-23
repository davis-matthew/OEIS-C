/*
 * A016781  a(n) = (3*n+1)^5.
 * https://oeis.org/A016781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^5
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 27, u64 n <= 2376,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first */
    seq_t xn = (seq_t)n;
    /* Compute 3*n + 1 */
    seq_t v = xn * (seq_t)3 + (seq_t)1;
    /* Compute v^2, v^3, and then v^5 = v^3 * v^2 */
    seq_t sq  = v * v;        /* v^2 */
    seq_t cub = sq * v;       /* v^3 */
    return cub * sq;          /* v^5 */
}
