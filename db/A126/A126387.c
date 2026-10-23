/*
 * A126387  Read binary expansion of n from the left; keep track of the excess
 *          of 1's over 0's that have been seen so far; sequence gives
 *          maximum(excess of 1's over 0's).
 * https://oeis.org/A126387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) returns the maximum difference between the number of 1's
 *            and the number of 0's seen when scanning the binary expansion of
 *            n from the most‑significant bit to the least‑significant bit. If
 *            n = 0, a(0) = 0. For all n >= 1, a(n) is computed by iterating
 *            over the bits of n from the highest to the...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned floor_log2_idx(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* binary "0" gives excess 0 */

    const unsigned msb = floor_log2_idx(n);
    int64_t diff = 0, maxdiff = 0;    /* running excess and its maximum */

    for (int i = (int)msb; i >= 0; --i) {
        if ((n >> i) & 1ULL)
            ++diff;
        else
            --diff;

        if (diff > maxdiff)
            maxdiff = diff;
    }
    return (seq_t)maxdiff;
}
