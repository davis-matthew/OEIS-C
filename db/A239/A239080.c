/*
 * A239080  Number of self-inverse permutations p on [n] with displacement of
 *          elements restricted by 8: |p(i)-i| <= 8.
 * https://oeis.org/A239080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of self‑inverse permutations (involutions) on [n]
 *            where |p(i)-i| <= 8 for all i. (OEIS A239080)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdint.h>

#define K 8
#define MASK_SZ (1 << K)

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* state mask: bit d (0‑based) indicates whether vertex (i-d-1)
       (i.e., i‑d‑1) is currently free and can still be matched
       with a future vertex.  The mask always keeps the last K free
       vertices.  */
    const int mask_lim = MASK_SZ - 1;
    seq_t cur[MASK_SZ];
    seq_t nxt[MASK_SZ];

    /* initialise */
    for (int i = 0; i < MASK_SZ; ++i) cur[i] = 0;
    cur[0] = 1;  /* before first vertex, no pending free vertices */

    idx_t i;
    for (i = 1; i <= n; ++i) {
        /* zero next */
        for (int j = 0; j < MASK_SZ; ++j) nxt[j] = 0;

        for (int s = 0; s < MASK_SZ; ++s) {
            seq_t val = cur[s];
            if (!val) continue;

            int base = ((s << 1) & mask_lim);  /* shift for the next step */

            /* Option 1: leave i free (fixed later or fixed point) */
            int m = base | 1;       /* set bit 0 to 1 */
            nxt[m] += val;

            /* Option 2: match i with a previous free vertex at distance d */
            for (int d = 1; d <= K; ++d) {
                if ((s >> (d - 1)) & 1) {      /* that vertex is free */
                    int m2 = base & ~(1 << d); /* clear the matched vertex */
                    m2 &= mask_lim;
                    nxt[m2] += val;
                }
            }
        }

        /* swap arrays */
        for (int j = 0; j < MASK_SZ; ++j) cur[j] = nxt[j];
    }

    /* Sum over all final states: free vertices become fixed points. */
    seq_t result = 0;
    for (int s = 0; s < MASK_SZ; ++s) result += cur[s];
    return result;
}
