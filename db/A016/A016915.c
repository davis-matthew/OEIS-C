/*
 * A016915  a(n) = (6*n)^7.
 * https://oeis.org/A016915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 94, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t C = 279936; /* 6^7 */
    seq_t prod = 1;

    /* Compute n^7 by repeated multiplication */
    for (idx_t i = 0; i < 7; ++i)
        prod *= (seq_t)n;

    return prod * C;
}
