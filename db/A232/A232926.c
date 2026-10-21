/*
 * A232926  Number of 7Xn 0..2 arrays with no element x(i,j) adjacent to value
 *          2-x(i,j) horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n arrays over {0,1,2} with no cell adjacent
 *            to value 2‑cell (horizontally, diagonally or antidiagonally).
 *            Computed by dynamic programming on column states
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   ok_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ok_pair.h"

#define STATES 2187                     /* 3^7 possible column states */
#define MAX_CACHE 20000                 /* cache depth sufficient for all tests */

static uint8_t digits[STATES][7];       /* digits of each state */
static uint16_t *adjacency = NULL;      /* flattened adjacency list */
static uint32_t *adjOffset = NULL;      /* start index of each state's list */

static seq_t dp_cur[STATES];            /* DP for current width   */
static seq_t dp_next[STATES];           /* DP for next width     */
static seq_t *dp_cur_ptr = NULL;        /* pointer to the active DP array */
static seq_t *dp_next_ptr = NULL;

static seq_t ansCache[MAX_CACHE + 1];   /* memoised a(n) values (1‑based) */
static idx_t cacheSize = 0;             /* largest n processed so far */

/* ----------------------------------------------------------------------
   Build the digit table and transition graph the first time we are called
   ---------------------------------------------------------------------- */
static void init_tables(void)
{
    if (adjacency != NULL) return;          /* already initialised */

    /* build digits for all 2187 states */
    for (int s = 0; s < STATES; ++s) {
        int v = s;
        for (int r = 0; r < 7; ++r) {
            digits[s][r] = (uint8_t)(v % 3);
            v /= 3;
        }
    }

    /* first pass: count outgoing edges per state */
    int *cnt = (int*)malloc(STATES * sizeof(int));
    if (!cnt) return;                       /* unlikely */
    for (int i = 0; i < STATES; ++i) cnt[i] = 0;

    for (int a = 0; a < STATES; ++a) {
        for (int b = 0; b < STATES; ++b) {
            bool ok = true;
            for (int r = 0; r < 7; ++r) {
                if (!ok_pair(digits[a][r], digits[b][r])) { ok = false; break; }
                if (r > 0   && !ok_pair(digits[a][r], digits[b][r-1])) { ok = false; break; }
                if (r < 6   && !ok_pair(digits[a][r], digits[b][r+1])) { ok = false; break; }
            }
            if (ok) cnt[a]++;
        }
    }

    /* allocate offset array */
    adjOffset = (uint32_t*)malloc((STATES + 1) * sizeof(uint32_t));
    if (!adjOffset) { free(cnt); return; }
    adjOffset[0] = 0;
    for (int i = 0; i < STATES; ++i) {
        adjOffset[i + 1] = adjOffset[i] + (uint32_t)cnt[i];
    }
    uint32_t totalTrans = adjOffset[STATES];

    /* allocate adjacency storage */
    adjacency = (uint16_t*)malloc(totalTrans * sizeof(uint16_t));
    if (!adjacency) { free(cnt); free(adjOffset); return; }

    /* second pass: fill adjacency lists */
    int *fillPos = (int*)malloc(STATES * sizeof(int));
    for (int i = 0; i < STATES; ++i) fillPos[i] = cnt[i] = adjOffset[i];

    for (int a = 0; a < STATES; ++a) {
        for (int b = 0; b < STATES; ++b) {
            bool ok = true;
            for (int r = 0; r < 7; ++r) {
                if (!ok_pair(digits[a][r], digits[b][r])) { ok = false; break; }
                if (r > 0   && !ok_pair(digits[a][r], digits[b][r-1])) { ok = false; break; }
                if (r < 6   && !ok_pair(digits[a][r], digits[b][r+1])) { ok = false; break; }
            }
            if (ok) {
                adjacency[fillPos[a]++] = (uint16_t)b;
            }
        }
    }

    free(cnt);
    free(fillPos);
}

/* ----------------------------------------------------------------------
   Initialise DP arrays if not already done
   ---------------------------------------------------------------------- */
static void init_dp(void)
{
    if (dp_cur_ptr) return;
    dp_cur_ptr = dp_cur;
    dp_next_ptr = dp_next;
    for (int i = 0; i < STATES; ++i) {
        dp_cur_ptr[i] = (seq_t)1;            /* each column alone */
        dp_next_ptr[i] = (seq_t)0;
    }
}

/* ----------------------------------------------------------------------
   Expand the cached result set up to at least n
   ---------------------------------------------------------------------- */
static void ensure_n(idx_t n)
{
    if (n <= cacheSize) return;            /* already have it */

    init_tables();
    init_dp();

    if (cacheSize == 0) {
        ansCache[1] = (seq_t)STATES;       /* a(1) = 2187 */
        cacheSize = 1;
    }

    /* compute missing terms */
    for (idx_t cur = cacheSize + 1; cur <= n; ++cur) {
        /* reset dp_next */
        for (int i = 0; i < STATES; ++i) dp_next_ptr[i] = (seq_t)0;

        /* transitions from dp_cur to dp_next */
        for (int a = 0; a < STATES; ++a) {
            seq_t val = dp_cur_ptr[a];
            if (!val) continue;
            uint32_t start = adjOffset[a];
            uint32_t end   = adjOffset[a + 1];
            for (uint32_t p = start; p < end; ++p) {
                dp_next_ptr[adjacency[p]] += val;
            }
        }

        /* compute total arrays for width cur */
        seq_t total = (seq_t)0;
        for (int i = 0; i < STATES; ++i) total += dp_next_ptr[i];
        ansCache[cur] = total;

        /* swap dp arrays for the next iteration */
        seq_t *tmp = dp_cur_ptr;
        dp_cur_ptr = dp_next_ptr;
        dp_next_ptr = tmp;
    }

    cacheSize = n;
}

/* ----------------------------------------------------------------------
   Compute value for arbitrary large n (without caching beyond limit)
   ---------------------------------------------------------------------- */
static seq_t compute_direct(idx_t n)
{
    /* initialise dp arrays for width 1 */
    for (int i = 0; i < STATES; ++i) dp_cur_ptr[i] = (seq_t)1;

    seq_t total = (seq_t)0;
    for (idx_t cur = 2; cur <= n; ++cur) {
        for (int i = 0; i < STATES; ++i) dp_next_ptr[i] = (seq_t)0;

        for (int a = 0; a < STATES; ++a) {
            seq_t val = dp_cur_ptr[a];
            if (!val) continue;
            uint32_t start = adjOffset[a];
            uint32_t end   = adjOffset[a + 1];
            for (uint32_t p = start; p < end; ++p) {
                dp_next_ptr[adjacency[p]] += val;
            }
        }

        if (cur == n) {
            total = 0;
            for (int i = 0; i < STATES; ++i) total += dp_next_ptr[i];
        }

        seq_t *tmp = dp_cur_ptr;
        dp_cur_ptr = dp_next_ptr;
        dp_next_ptr = tmp;
    }
    return total;
}

/* ----------------------------------------------------------------------
   Public API
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= MAX_CACHE) {
        ensure_n(n);
        return ansCache[n];
    } else {
        init_tables();
        init_dp();
        return compute_direct(n);
    }
}
