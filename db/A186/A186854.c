/*
 * A186854  Number of 5-step knight's tours on an (n+2) X (n+2) board summed
 *          over all starting positions.
 * https://oeis.org/A186854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2696*n^2 - 11224*n + 9904 (for n > 5); The first five
 *            values are hard‑coded as they are not covered by the empirical
 *            formula. This implementation uses an unsigned 64‑bit
 *            intermediate to avoid overflow on 32‑bit seq_t builds
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and the special cases n = 1…5 */
    switch (n) {
    case 1: return (seq_t)16;
    case 2: return (seq_t)400;
    case 3: return (seq_t)2800;
    case 4: return (seq_t)9328;
    case 5: return (seq_t)21480;
    default: /* n > 5 */
        /* Use a 64‑bit intermediate to compute the quadratic formula */
        unsigned long long nn = (unsigned long long)n;
        unsigned long long val = 2696ULL * nn * nn
                               - 11224ULL * nn
                               + 9904ULL;
        return (seq_t)val;
    }
}
