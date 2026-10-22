/*
 * A006819  Trails of length n on cubic lattice.
 * https://oeis.org/A006819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   no closed form; a(0) = 1, and for n >= 1 the code counts every
 *            n-step walk from the origin on the cubic lattice that never
 *            reuses an edge (a depth-first search over a grid of used-edge
 *            bits). To save work it uses symmetry: it fixes the first step as
 *            +x (weight 6), the first step off the x axis as...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

static unsigned char *grid_used = NULL;
static long grid_off[3];

static unsigned long long trail_dfs(long p, idx_t rem, int phase)
{
    unsigned long long total = 0;
    int d;
    for (d = 0; d < 6; d++) {
        int axis = d >> 1, neg = d & 1;
        unsigned long long w = 1;
        int nphase = phase;
        long e, q;
        unsigned char bit;
        if (phase == 0) {
            if (axis == 0) w = 1;
            else if (d == 2) { w = 4; nphase = 1; }
            else continue;
        } else if (phase == 1) {
            if (axis < 2) w = 1;
            else if (d == 4) { w = 2; nphase = 2; }
            else continue;
        }
        e = neg ? p - grid_off[axis] : p;
        bit = (unsigned char)(1u << axis);
        if (grid_used[e] & bit) continue;
        if (rem == 1) {
            total += w;
        } else {
            q = neg ? p - grid_off[axis] : p + grid_off[axis];
            grid_used[e] |= bit;
            total += w * trail_dfs(q, rem - 1, nphase);
            grid_used[e] &= (unsigned char)~bit;
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    static unsigned long long *memo = NULL;
    static bool *have = NULL;
    static idx_t memo_len = 0;
    unsigned long long res;

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    if (n < memo_len && have[n]) return u128_to_seq((unsigned __int128)memo[n]);

    {
        long S = (long)(2 * n + 3);
        size_t cells = (size_t)S * (size_t)S * (size_t)S;
        long center = (long)(n + 1) * S * S + (long)(n + 1) * S + (long)(n + 1);
        grid_used = (unsigned char *)calloc(cells, 1);
        grid_off[0] = S * S;
        grid_off[1] = S;
        grid_off[2] = 1;
        /* first step fixed to +x, multiplied by 6 */
        grid_used[center] |= 1;
        res = 6ULL * trail_dfs(center + grid_off[0], n - 1, 0);
        free(grid_used);
        grid_used = NULL;
    }

    if (n >= memo_len) {
        idx_t nl = n + 16, i;
        unsigned long long *nm = (unsigned long long *)malloc(nl * sizeof(unsigned long long));
        bool *nh = (bool *)malloc(nl * sizeof(bool));
        for (i = 0; i < nl; i++) {
            if (i < memo_len) { nm[i] = memo[i]; nh[i] = have[i]; }
            else { nm[i] = 0; nh[i] = false; }
        }
        free(memo);
        free(have);
        memo = nm;
        have = nh;
        memo_len = nl;
    }
    memo[n] = res;
    have[n] = true;
    return u128_to_seq((unsigned __int128)res);
}
