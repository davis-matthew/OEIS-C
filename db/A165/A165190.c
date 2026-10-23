/*
 * A165190  G.f.: 1/((1-x^4)*(1-x^5)).
 * https://oeis.org/A165190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+4)/4) - floor((n+4)/5)
 * Range:     u8 n <= 697, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 4 and 5 */

seq_t a(idx_t n)
{
    /* Compute (n+4)/4 and (n+4)/5 using seq_t arithmetic. */
    seq_t t = (seq_t)n + 4;
    return t / 4 - t / 5;
}
