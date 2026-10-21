/*
 * A207657  Number of nX4 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   use a linear‑recurrence DP on rows a(1) = 10 a(2) = 100 For n≥3,
 *            a(n) is computed by propagating counts of allowed row pairs
 *            through the height using vertical constraints (no 001 or 100 in
 *            any column). The DP has 10×10 states (10 admissible 4‑bit rows)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   ok_row, ok_vert
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ok_row.h"
#include "../utils/ok_vert.h"

/* ------------------------------------------------------------------ */
/* Global state – initialized on first call.                          */
/* ------------------------------------------------------------------ */

#define ALLOW_COUNT 10

static bool initialized = false;
static int allowed[ALLOW_COUNT];      /* list of admissible rows 0–15   */
static int row_index[16];             /* mapping row value → index     */
static seq_t *cache = NULL;           /* cache a[1..max]               */
static size_t cache_cap = 0;          /* capacity of cache             */
static idx_t cur_n = 2;               /* highest n computed so far     */
static seq_t dp[ALLOW_COUNT][ALLOW_COUNT];
static seq_t nextdp[ALLOW_COUNT][ALLOW_COUNT];

/* Initialise admissible rows and base DP. */
static void init(void)
{
    int cnt = 0;
    for (int r = 0; r < 16; ++r)
        if (ok_row(r))
            allowed[cnt++] = r;

    for (int i = 0; i < 16; ++i)
        row_index[i] = -1;
    for (int i = 0; i < cnt; ++i)
        row_index[allowed[i]] = i;

    /* base DP for n = 2: each pair of allowed rows contributes 1. */
    for (int i = 0; i < ALLOW_COUNT; ++i)
        for (int j = 0; j < ALLOW_COUNT; ++j)
            dp[i][j] = (seq_t)1;

    /* allocate cache and store a(1) and a(2). */
    cache_cap = 3ull;                 /* indices 0,1,2 */
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)10;   /* a(1) */
    cache[2] = (seq_t)100;  /* a(2) */

    cur_n = 2;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n < 1)
        return (seq_t)0;

    /* enlarge cache if needed */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* cached result exists */
    if (n <= cur_n)
        return cache[n];

    /* compute DP for n = cur_n+1 … n */
    for (idx_t x = cur_n + 1; x <= n; ++x) {
        /* zero nextdp */
        for (int i = 0; i < ALLOW_COUNT; ++i)
            for (int j = 0; j < ALLOW_COUNT; ++j)
                nextdp[i][j] = (seq_t)0;

        /* transition from current dp to nextdp */
        for (int ia = 0; ia < ALLOW_COUNT; ++ia) {
            int ra = allowed[ia];
            for (int ib = 0; ib < ALLOW_COUNT; ++ib) {
                int rb = allowed[ib];
                seq_t ways = dp[ia][ib];
                if (!ways)
                    continue;
                for (int ic = 0; ic < ALLOW_COUNT; ++ic) {
                    int rc = allowed[ic];
                    if (!ok_vert(ra, rb, rc))
                        continue;
                    nextdp[ib][ic] += ways;
                }
            }
        }

        /* total for current n */
        seq_t total = (seq_t)0;
        for (int i = 0; i < ALLOW_COUNT; ++i)
            for (int j = 0; j < ALLOW_COUNT; ++j)
                total += nextdp[i][j];

        cache[x] = total;

        /* prepare dp for next iteration */
        for (int i = 0; i < ALLOW_COUNT; ++i)
            for (int j = 0; j < ALLOW_COUNT; ++j)
                dp[i][j] = nextdp[i][j];
    }

    cur_n = n;
    return cache[n];
}
