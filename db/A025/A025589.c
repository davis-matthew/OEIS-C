/*
 * A025589  Number of n-move self-avoiding knight paths on 5 X 5 board,
 *          beginning at corner.
 * https://oeis.org/A025589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, walk
 * Formula:   a(n) = Number of n‑move self‑avoiding knight paths on a 5×5
 *            board starting from the corner (0,0). Computed by exhaustive DFS
 *            up to 24 moves
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define BOARD 5
#define NMAX   24          /* maximum possible move count on a 5×5 board */

/* Orthogonal/diagonal knight moves */
static const int dx[8] = { -2,-2,-1,-1, 1, 1, 2, 2 };
static const int dy[8] = { -1, 1,-2, 2,-2, 2,-1, 1 };

/* Memoisation table: a[n] for 0 ≤ n ≤ 24 */
static seq_t memo[NMAX + 1];

/* One‑time initialisation flag */
static int initialized = 0;

/* Depth‑first search exploring all self‑avoiding knight paths */
static void dfs(unsigned int mask, int depth, int x, int y)
{
    /* Count the path of length 'depth' (including zero‑move path) */
    memo[depth]++;

    if (depth == NMAX) return;        /* cannot extend further */

    for (int k = 0; k < 8; ++k) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (nx < 0 || ny < 0 || nx >= BOARD || ny >= BOARD) continue;

        unsigned int idx = (unsigned int)(nx * BOARD + ny);
        if (mask & (1u << idx)) continue;   /* already visited */

        dfs(mask | (1u << idx), depth + 1, nx, ny);
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        for (int i = 0; i <= NMAX; ++i) memo[i] = 0;
        dfs(1u << 0, 0, 0, 0);     /* start from corner (0,0) */
        initialized = 1;
    }
    if (n > NMAX) return 0;
    return memo[(size_t)n];
}
