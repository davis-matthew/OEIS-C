/*
 * A034188  Number of binary codes of length 3 with n words.
 * https://oeis.org/A034188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary codes of length 3 with n words, up to
 *            permutations of the three coordinate positions and translations
 *            (XOR by a binary 3‑tuple). The group is the affine group of the
 *            3‑dimensional binary cube with only coordinate permutations in
 *            its linear part (48 elements)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* popcount of an 8‑bit integer */
static inline int popcnt8(unsigned int x)
{
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

/* return j permuted according to the permutation p (0…5) of the
   three coordinate positions.  The bits of j are (b2,b1,b0) with
   b2 = bit 2 (value 4), b1 = bit 1 (value 2), b0 = bit 0 (value 1). */
static inline int permute(int j, int p)
{
    switch (p) {
        case 0: return ((j >> 2) & 1) << 2 | ((j >> 1) & 1) << 1 | (j & 1);
        case 1: return ((j >> 2) & 1) << 2 | (j & 1) << 1 | ((j >> 1) & 1);
        case 2: return (j & 1) << 2 | ((j >> 2) & 1) << 1 | ((j >> 1) & 1);
        case 3: return (j & 1) << 2 | ((j >> 1) & 1) << 1 | ((j >> 2) & 1);
        case 4: return ((j >> 1) & 1) << 2 | ((j >> 2) & 1) << 1 | (j & 1);
        default:/* case 5 */return ((j >> 1) & 1) << 2 | (j & 1) << 1 | ((j >> 2) & 1);
    }
}

/* Number of binary codes of length 3 with n words. */
/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n > 8) return 0;
    bool seen[256] = {0};
    int orbits = 0;

    for (int mask = 0; mask < 256; ++mask) {
        if (popcnt8(mask) != (int)n) continue;

        uint8_t canon = (uint8_t)mask;

        for (int p = 0; p < 6; ++p) {            /* 6 permutations   */
            for (int t = 0; t < 8; ++t) {        /* 8 translations  */
                uint8_t newmask = 0;
                for (int j = 0; j < 8; ++j) {
                    if (mask & (1 << j)) {
                        int pj = permute(j, p) ^ t;   /* permute then translate */
                        newmask |= 1 << pj;
                    }
                }
                if (newmask < canon) canon = newmask;
            }
        }

        if (!seen[canon]) {
            seen[canon] = true;
            ++orbits;
        }
    }

    return (seq_t)orbits;
}
