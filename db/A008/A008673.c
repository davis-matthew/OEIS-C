/*
 * A008673  Expansion of 1/((1-x)*(1-x^3)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A008673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 24*n^2 + 171*n + 630)/630)
 * Range:     u8 n <= 46, u16 n <= 337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the numerator using 64‑bit arithmetic to avoid overflow
     * even when seq_t is only 32 bits.  n is at most a few thousand, so
     * the intermediate fits comfortably in unsigned long long. */
    unsigned long long nn   = (unsigned long long)n;
    unsigned long long num  = nn * nn * nn
                          + 24ULL * nn * nn
                          + 171ULL * nn
                          + 630ULL;
    return (seq_t)(num / 630ULL);
}
