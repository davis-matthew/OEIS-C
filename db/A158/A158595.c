/*
 * A158595  a(n) = 361*n^2 - 19.
 * https://oeis.org/A158595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 361*n^2 - 19
 * Range:     u8 none, u16 n <= 13, u32 n <= 3449, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for the intermediate to avoid overflow when
       computing 361*n*n for any value of n that fits in seq_t. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val = 361ULL * nn * nn - 19ULL;
    return (seq_t)val;
}
