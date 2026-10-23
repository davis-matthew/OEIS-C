/*
 * A163889  Number of unlock patterns of length n for the Android operating
 *          system.
 * https://oeis.org/A163889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of Android unlock patterns of length n (offset 0).
   For n = 0..9 the values are counted by depth‑first search.
   For n > 9 the answer is 0. */

#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
static seq_t counts[10];

/* jump[i][j] is the index of the intermediate dot that must be
   already visited to move directly from i to j, or -1 if none. */
static int jump[9][9];

/* Depth‑first search that walks all legal partial patterns.
   node  : current position (0..8)
   mask  : bitmask of visited nodes
   len   : current length (>= 1) */
static void dfs(int node, unsigned int mask, idx_t len)
{
    /* count the current partial pattern */
    counts[(size_t)len] += (seq_t)1;

    if (len == 9)
        return;

    for (int next = 0; next < 9; ++next) {
        if (mask & (1u << next))
            continue;
        int mid = jump[node][next];
        if (mid != -1 && !(mask & (1u << mid)))
            continue;
        dfs(next, mask | (1u << next), len + 1);
    }
}

/* Build the jump table and pre‑compute all counts. */
static void init_counts(void)
{
    if (initialized)
        return;

    /* initialise counts to zero */
    for (int i = 0; i < 10; ++i)
        counts[i] = 0;

    /* build jump table */
    for (int i = 0; i < 9; ++i) {
        int xi = i / 3, yi = i % 3;
        for (int j = 0; j < 9; ++j) {
            if (i == j) {
                jump[i][j] = -1;
                continue;
            }
            int xj = j / 3, yj = j % 3;
            int dx = xj - xi, dy = yj - yi;
            int adx = dx > 0 ? dx : -dx;
            int ady = dy > 0 ? dy : -dy;
            if ((adx == 2 && ady == 0) ||
                (adx == 0 && ady == 2) ||
                (adx == 2 && ady == 2)) {
                int midx = xi + dx / 2;
                int midy = yi + dy / 2;
                jump[i][j] = midx * 3 + midy;
            } else {
                jump[i][j] = -1;
            }
        }
    }

    /* count all patterns starting from each initial node */
    counts[0] = (seq_t)1;   /* empty pattern */
    for (int start = 0; start < 9; ++start)
        dfs(start, 1u << start, 1);

    initialized = true;
}

seq_t a(idx_t n)
{
    init_counts();
    if (n > 9)
        return (seq_t)0;
    return counts[(size_t)n];
}
