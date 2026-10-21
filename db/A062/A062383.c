/*
 * A062383  a(0) = 1: for n>0, a(n) = 2^floor(log_2(n)+1) or a(n) =
 *          2*a(floor(n/2)).
 * https://oeis.org/A062383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = smallest power of 2 strictly greater than n
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the least power of two that exceeds n.
       This works for all n that fit into idx_t. */
    idx_t m   = (idx_t)n + 1;   /* target value */
    idx_t pow = 1;              /* current power of two */

    /* Find the smallest pow >= m, i.e. pow >= n+1. */
    while (pow < m)
        pow <<= 1;

    return (seq_t)pow;
}
