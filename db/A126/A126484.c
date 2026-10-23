/*
 * A126484  Number of base 16 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base 16 n‑digit numbers (leading zeros allowed)
 *            whose adjacent digits differ by at most 3. Computed by a simple
 *            DP over the 16‑vertex graph where vertices 0–15 are connected
 *            iff |i−j| ≤ 3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* adjacency lists for the 16 vertices */
static const int neigh[16][7] = {
    {0, 1, 2, 3, -1, -1, -1},          /* 0 */
    {0, 1, 2, 3, 4, -1, -1},           /* 1 */
    {0, 1, 2, 3, 4, 5, -1},            /* 2 */
    {0, 1, 2, 3, 4, 5, 6},             /* 3 */
    {1, 2, 3, 4, 5, 6, 7},             /* 4 */
    {2, 3, 4, 5, 6, 7, 8},             /* 5 */
    {3, 4, 5, 6, 7, 8, 9},             /* 6 */
    {4, 5, 6, 7, 8, 9,10},             /* 7 */
    {5, 6, 7, 8, 9,10,11},             /* 8 */
    {6, 7, 8, 9,10,11,12},             /* 9 */
    {7, 8, 9,10,11,12,13},             /*10 */
    {8, 9,10,11,12,13,14},             /*11 */
    {9,10,11,12,13,14,15},             /*12 */
    {10,11,12,13,14,15,-1},            /*13 */
    {11,12,13,14,15,-1,-1},            /*14 */
    {12,13,14,15,-1,-1,-1}             /*15 */
};
static const int neigh_cnt[16] = {
    4,5,6,7,7,7,7,7,7,7,7,7,7,6,5,4
};

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    seq_t cur[16], next[16];

    /* initial DP state: all one‑digit sequences */
    for (int d = 0; d < 16; ++d)
        cur[d] = (seq_t)1;

    /* perform (n‑1) transitions */
    for (idx_t step = 1; step < n; ++step) {
        /* zero next */
        for (int k = 0; k < 16; ++k)
            next[k] = (seq_t)0;

        /* propagate counts to neighbours */
        for (int src = 0; src < 16; ++src) {
            seq_t val = cur[src];
            if (val == (seq_t)0) continue;
            for (int i = 0; i < neigh_cnt[src]; ++i) {
                int dst = neigh[src][i];
                next[dst] += val;
            }
        }

        /* copy next into cur */
        for (int k = 0; k < 16; ++k)
            cur[k] = next[k];
    }

    /* sum over all end states */
    seq_t total = (seq_t)0;
    for (int k = 0; k < 16; ++k)
        total += cur[k];

    return total;
}
