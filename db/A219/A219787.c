/*
 * A219787  Least number such that in the interval [n+1, a(n)] there are n
 *          integers b(i) with i | b(i) for i = 1 to n.
 * https://oeis.org/A219787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The function computes OEIS A219787 by binary searching the
 *            smallest a(n) and testing for the existence of a perfect
 *            matching in the bipartite graph with left vertices 1..n and
 *            right vertices n+1..a. The feasibility test uses Hopcroft–Karp
 * Range:     u8 n <= 99, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   _bfs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_bfs.h"

static bool _dfs(idx_t i, idx_t n, idx_t a,
                 int *dist, int *matchL, int *matchR)
{
    const idx_t n1 = n + 1;
    idx_t first = ((n1 + i - 1) / i) * i;
    for (idx_t m = first; m <= a; m += i)
    {
        idx_t r = m - n1;
        idx_t j = matchR[r];
        if (j == -1 ||
            (dist[j] == dist[i] + 1 && _dfs(j, n, a, dist, matchL, matchR)))
        {
            matchL[i] = r;
            matchR[r] = i;
            return true;
        }
    }
    dist[i] = -1;
    return false;
}

static bool _feasible(idx_t n, idx_t a)
{
    /* arrays indexed from 0..n (left) and 0..a-n (right) */
    int *matchL = (int*)malloc((n + 1) * sizeof(int));
    int *matchR = (int*)malloc((a - n + 1) * sizeof(int));
    int *dist   = (int*)malloc((n + 1) * sizeof(int));
    if (!matchL || !matchR || !dist)
        return false; /* allocation failure, treat as infeasible */

    for (idx_t i = 1; i <= n; ++i) matchL[i] = -1;
    matchL[0] = -1; /* unused */
    for (idx_t r = 0; r <= a - n; ++r) matchR[r] = -1;
    for (idx_t i = 1; i <= n; ++i) dist[i] = -1;
    dist[0] = -1;

    int matched = 0;
    while (_bfs(n, a, dist, matchL, matchR))
    {
        for (idx_t i = 1; i <= n; ++i)
        {
            if (matchL[i] == -1 && _dfs(i, n, a, dist, matchL, matchR))
                ++matched;
        }
    }

    bool ok = (matched == (int)n);
    free(matchL); free(matchR); free(dist);
    return ok;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset < 1 */

    /* Find an upper bound by doubling */
    idx_t a_high = n + 1;
    while (!_feasible(n, a_high))
    {
        idx_t new_high = a_high << 1;
        if (new_high <= a_high) /* overflow */
            break;
        a_high = new_high;
    }

    /* Binary search between n+1 and a_high */
    idx_t low = n + 1;
    idx_t high = a_high;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (_feasible(n, mid))
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
