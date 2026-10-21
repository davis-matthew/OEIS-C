/*
 * A238916  Number of self-inverse permutations p on [n] where the maximal
 *          displacement of an element equals 5.
 * https://oeis.org/A238916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of self‑inverse permutations p on [n] where the
 *            maximal displacement of an element equals 5
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define K 5
#define MASKS (1 << K)

seq_t a(idx_t n)
{
    /* DP over positions with a bitmask of length K where the i‑th bit (1‑based) tells if the
       (pos+i‑1) vertex is already matched to a vertex on the left.
       The second dimension "used" indicates whether a 5‑edge has already been chosen. */
    seq_t cur[MASKS * 2] = {0};
    seq_t nxt[MASKS * 2] = {0};

    cur[0] = 1;               /* mask = 0, used = 0 */

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear next array */
        for (int i = 0; i < MASKS * 2; ++i) {
            nxt[i] = 0;
        }

        for (int mask = 0; mask < MASKS; ++mask) {
            for (int used = 0; used <= 1; ++used) {
                seq_t val = cur[mask * 2 + used];
                if (!val) continue;

                if (mask & 1) {   /* current position already matched */
                    int newMask = mask >> 1;
                    nxt[newMask * 2 + used] += val;
                } else {
                    /* leave current position unmatched */
                    int newMask = mask >> 1;
                    nxt[newMask * 2 + used] += val;

                    /* try to match current position with a vertex pos+d (1 <= d <= K) */
                    for (int d = 1; d <= K; ++d) {
                        if (pos + d > n) break;          /* pair would go out of bounds */
                        if ((mask >> d) & 1) continue;   /* partner already matched */

                        int newMask2 = (mask | (1 << d)) >> 1;
                        int newUsed  = used | ((d == K) ? 1 : 0);
                        nxt[newMask2 * 2 + newUsed] += val;
                    }
                }
            }
        }

        /* copy nxt to cur for next iteration */
        for (int i = 0; i < MASKS * 2; ++i) {
            cur[i] = nxt[i];
        }
    }

    /* result: matchings with at least one 5‑edge (used = 1) and no pending vertices beyond n (mask = 0) */
    return cur[0 * 2 + 1];
}
