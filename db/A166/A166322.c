/*
 * A166322  The number of times a point sum n is attained in all 7^6
 *          permutations of throwing 7 dice.
 * https://oeis.org/A166322
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of ordered 7‑tuples of {1..6} whose * sum equals
 *            n. Equivalently, a(n) is the coefficient of x^n in * (x + x² + …
 *            + x⁶)⁷. The first term is a(7) = 1. For n < 7 or * n > 42 the
 *            value is 0
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* number of dice and faces */
    const idx_t dice  = 7;
    const idx_t faces = 6;
    const idx_t maxsum = dice * faces;   /* 42 */

    static seq_t dist[43];            /* indices 0..42 */
    static int initialized = 0;

    if (!initialized) {
        /* initialise to all zeros */
        for (idx_t i = 0; i <= maxsum; ++i)
            dist[i] = (seq_t)0;
        dist[0] = (seq_t)1;          /* empty sum */

        /* convolve dice times */
        for (idx_t d = 0; d < dice; ++d) {
            seq_t newDist[43] = {0};

            for (idx_t s = 0; s <= maxsum; ++s) {
                seq_t cur = dist[s];
                if (cur == 0) continue;
                for (idx_t f = 1; f <= faces; ++f) {
                    idx_t ns = s + f;
                    if (ns <= maxsum)
                        newDist[ns] += cur;
                }
            }

            /* copy newDist back to dist */
            for (idx_t i = 0; i <= maxsum; ++i)
                dist[i] = newDist[i];
        }

        initialized = 1;
    }

    if (n < 7 || n > maxsum)
        return (seq_t)0;
    return dist[n];
}
