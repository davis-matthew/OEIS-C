/*
 * A063654  Number of ways to tile a 10 X n rectangle with 1 X 1 and 2 X 2
 *          tiles.
 * https://oeis.org/A063654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   dfs_fill
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063654 – Number of ways to tile a 10×n rectangle with 1×1 and 2×2
   squares.  The sequence is computed by dynamic programming:
   A state is a 10‑bit mask indicating which cells in a column are
   already occupied by the left half of a 2×2 tile.  For each state we
   pre‑compute all possible next states together with the number of ways
   to realise them.  Scanning n columns applies these transitions and
   returns the number of tilings that end in the empty state. */

/* The function signature is prescribed by the build system.  Types
   seq_t and idx_t are defined elsewhere; they are used implicitly in
   this file. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/dfs_fill.h"

/* Maximal number of column states: 2^10 = 1024 */
#define MAX_STATES (1U << 10)

/* Transition table entry: next state and count of ways. */
struct TransEntry {
    uint16_t mask;   /* 0 … 1023 */
    seq_t    count;  /* number of ways to reach next state */
};

/* Per‑state transition list.  Initialized lazily on first call. */
static struct TransEntry *trans[MAX_STATES];
static size_t trans_len[MAX_STATES];
static int initialized = 0;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Lazy static initialisation of the transition table.  This function
    is called once, even if a() is called from multiple threads, as
    the standard guarantees that static local variables are initialised
    atomically.  The table is small enough to pre‑allocate per column.
*/
static void init_transitions(void)
{
    if (initialized) return;
    for (uint16_t s = 0; s < MAX_STATES; ++s) {
        seq_t tmp[MAX_STATES] = {0};
        dfs_fill(s, 0, tmp);
        size_t cnt = 0;
        for (size_t i = 0; i < MAX_STATES; ++i)
            if (tmp[i] != 0) ++cnt;
        trans_len[s] = cnt;
        trans[s] = (struct TransEntry *)malloc(cnt * sizeof(struct TransEntry));
        if (!trans[s]) abort();   /* fatal error if allocation fails */
        size_t idx = 0;
        for (size_t i = 0; i < MAX_STATES; ++i) {
            if (tmp[i] != 0) {
                trans[s][idx].mask  = (uint16_t)i;
                trans[s][idx].count = tmp[i];
                ++idx;
            }
        }
    }
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.  For each column we apply all possible
    transitions to update the DP table.  The result is the number
    of tilings that end in the empty state (mask 0). */
seq_t a(idx_t n)
{
    init_transitions();

    seq_t dp_cur[MAX_STATES] = {0};
    seq_t dp_next[MAX_STATES];

    dp_cur[0] = 1;          /* starting with an empty board */

    for (idx_t col = 0; col < n; ++col) {
        /* zero next DP row */
        for (size_t i = 0; i < MAX_STATES; ++i)
            dp_next[i] = 0;

        for (size_t s = 0; s < MAX_STATES; ++s) {
            if (dp_cur[s] == 0) continue;
            size_t len = trans_len[s];
            for (size_t j = 0; j < len; ++j) {
                const struct TransEntry *e = &trans[s][j];
                dp_next[e->mask] += dp_cur[s] * e->count;
            }
        }
        /* copy dp_next to dp_cur for next iteration */
        for (size_t i = 0; i < MAX_STATES; ++i)
            dp_cur[i] = dp_next[i];
    }
    return dp_cur[0];
}
