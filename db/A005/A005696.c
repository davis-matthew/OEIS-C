/*
 * A005696  Positions of remoteness 4 in Beans-Don't-Talk.
 * https://oeis.org/A005696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th position m >= 1 with remoteness
 *            exactly 4 in Beans-Don't-Talk. A move from m goes to the odd
 *            part of 3m+1 or of 3m-1, and 1 is the terminal position
 *            (remoteness 0). Remoteness is computed with the Steinhaus rule:
 *            an N-position has 1 + the least remoteness among its P-op...
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   bdt_oddpart
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bdt_oddpart.h"

/* remoteness of m if it is <= d, otherwise -1 */
static int bdt_rem(uint64_t m, int d)
{
    uint64_t opt[2];
    int i, minP = -1, maxN = 0;
    bool allKnown = true;
    if (m == 1) return 0;
    if (d == 0) return -1;
    opt[0] = bdt_oddpart(3 * m + 1);
    opt[1] = bdt_oddpart(3 * m - 1);
    for (i = 0; i < 2; i++) {
        int r = bdt_rem(opt[i], d - 1);
        if (r < 0) {
            allKnown = false;
        } else if ((r & 1) == 0) {
            if (minP < 0 || r < minP) minP = r;
        } else {
            if (r > maxN) maxN = r;
        }
    }
    if (minP >= 0) return minP + 1;
    if (allKnown) return maxN + 1;
    return -1;
}

static uint64_t *bdt_terms = NULL;
static idx_t bdt_count = 0, bdt_cap = 0;
static uint64_t bdt_next = 1;

seq_t a(idx_t n)
{
    while (bdt_count < n) {
        uint64_t m = bdt_next++;
        if (bdt_rem(m, 4) == 4) {
            if (bdt_count == bdt_cap) {
                idx_t nc = bdt_cap ? 2 * bdt_cap : 64;
                bdt_terms = (uint64_t *)realloc(bdt_terms, (size_t)nc * sizeof(uint64_t));
                bdt_cap = nc;
            }
            bdt_terms[bdt_count++] = m;
        }
    }
    return (seq_t)bdt_terms[n - 1];
}
