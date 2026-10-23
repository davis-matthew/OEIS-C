/*
 * A128998  Length of shortest addition-subtraction chain for n.
 * https://oeis.org/A128998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of a shortest addition‑subtraction chain
 * for n (n ≥ 1).
 * The algorithm uses iterative deepening depth‑first search with
 * simple pruning: the lower bound on remaining steps is the
 * number of doublings needed to reach n from the current maximum.
 *
 * Note: a(1) is defined to be 0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Recursive search.
 * chain[0 .. depth] holds the current chain, chain[0] = 1.
 * max_val is the largest element in the chain so far.
 * target is the value we want to reach.
 * limit is the maximum value allowed to be stored in vis[].
 * vis[v] == 1 means v has already been produced in this search path.
 */
static bool dfs(idx_t *chain, int depth, int max_depth,
                idx_t max_val, idx_t target, idx_t limit, char *vis)
{
    if (depth == max_depth)                 /* no more steps allowed */
        return false;

    /* Lower bound: if we double every remaining step, can we reach target? */
    idx_t cur = max_val;
    int need = 0;
    while (cur < target) {
        cur <<= 1;
        ++need;
    }
    if (depth + need > max_depth)
        return false;

    for (int i = 0; i <= depth; ++i) {
        for (int j = i; j <= depth; ++j) {
            idx_t a = chain[i];
            idx_t b = chain[j];

            /* addition */
            idx_t s = a + b;
            if (s == target)
                return true;
            if (s <= limit && !vis[s]) {
                vis[s] = 1;
                chain[depth + 1] = s;
                if (dfs(chain, depth + 1, max_depth,
                        max_val > s ? max_val : s, target, limit, vis)) {
                    vis[s] = 0;
                    return true;
                }
                vis[s] = 0;
            }

            /* subtraction (non‑zero) */
            idx_t d = (a > b) ? (a - b) : (b - a);
            if (d == target)
                return true;
            if (d > 0 && d <= limit && !vis[d]) {
                vis[d] = 1;
                chain[depth + 1] = d;
                if (dfs(chain, depth + 1, max_depth,
                        max_val > d ? max_val : max_val, target, limit, vis)) {
                    vis[d] = 0;
                    return true;
                }
                vis[d] = 0;
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n == 1u)
        return (seq_t)0;

    /* Upper bound from binary method: floor_log2(n) + popcount(n) – 1 */
    int ulp = 0;
    idx_t t = n;
    while (t >>= 1) ++ulp;                        /* floor_log2 */
    int pop = __builtin_popcountll((unsigned long long)n);
    int upper = ulp + pop - 1;
    if (upper < 0) upper = 0;

    /* Iterative deepening */
    for (int max_depth = 1; max_depth <= upper; ++max_depth) {
        idx_t limit = n * 2;                     /* generous bound */

        /* visited array: index 0 unused, 1..limit used */
        char *vis = (char *)calloc((size_t)(limit + 1), 1);
        if (!vis) abort();                        /* out of memory */
        vis[1] = 1;

        /* chain buffer */
        idx_t chain[200];                         /* depth never exceeds 200 */
        chain[0] = 1;

        bool found = dfs(chain, 0, max_depth, 1, n, limit, vis);
        free(vis);
        if (found)
            return (seq_t)max_depth;
    }
    /* Should never happen; return 0 as a fallback */
    return (seq_t)0;
}
