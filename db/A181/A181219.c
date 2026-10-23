/*
 * A181219  Number of n X 3 binary matrices with no three 1's adjacent in a
 *          line horizontally, vertically, diagonally or antidiagonally.
 * https://oeis.org/A181219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by dynamic programming from the definition:
   a(n) = number of n×3 binary matrices with no three 1's adjacent
   in any straight line (horizontal, vertical, or diagonal). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper: test whether three consecutive columns (with real columns)
   contain a line of three 1's.   prev==8 means dummy column (no check). */
static bool
valid_triplet(int prev, int curr, int next)
{
    if (prev == 8) /* dummy column – no check possible */
        return true;

    /* horizontal 3‑cells per row */
    for (int r = 0; r < 3; ++r) {
        if (((prev >> r) & 1) &&
            ((curr >> r) & 1) &&
            ((next >> r) & 1))
            return false;
    }

    /* main diagonal (∖) : bits 0,1,2 */
    if (((prev >> 0) & 1) &&
        ((curr >> 1) & 1) &&
        ((next >> 2) & 1))
        return false;

    /* anti‑diagonal (/): bits 2,1,0 */
    if (((prev >> 2) & 1) &&
        ((curr >> 1) & 1) &&
        ((next >> 0) & 1))
        return false;

    return true;
}

/*=============================================================
  Dynamic‑programming storage – kept static for incremental
  computation.  Total 72 states: (prev∈[0,8])×(curr∈[0,7]).
  State index = prev*8 + curr.   Dummy column encoded as 8.
  =============================================================*/

static seq_t *dp_prev = NULL;     /* values for length L */
static seq_t *dp_next = NULL;     /* temporary for length L+1 */
static const int DUMMY = 8;        /* sentinel for first column */
static const int STATES = 72;      /* 9*8 */

/* cache of a(n) for n>=1, 1‑based.  cache[0] = a(1). */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* current number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */

/* allocate or grow cache */
static void
grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) /* allocation failure – abort */
        exit(1);
    cache = new_arr;
    cache_cap = new_cap;
}

/* allocate dp arrays */
static void
init_dp(void)
{
    if (dp_prev)
        return; /* already done */
    dp_prev = (seq_t *)calloc(STATES, sizeof(seq_t));
    dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));
    if (!dp_prev || !dp_next) exit(1);
}

/* compute terms up to N (1‑based) */
static void
compute_up_to(size_t N)
{
    if (N <= cache_len)
        return;  /* already computed */

    /* ensure dp arrays ready */
    init_dp();

    /* initialise for n = 1 */
    if (cache_len == 0) {
        for (int col = 0; col < 8; ++col) {
            if (col == 7)   /* column 111 forbidden */
                continue;
            int idx = DUMMY * 8 + col;
            dp_prev[idx] = (seq_t)1;
        }
        /* sum over all counts -> a(1) */
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i)
            total += dp_prev[i];
        grow_cache(1);
        cache[0] = total;
        cache_len = 1;
    }

    /* extend from current cache_len to N */
    for (size_t len = cache_len + 1; len <= N; ++len) {
        /* reset dp_next */
        for (int i = 0; i < STATES; ++i)
            dp_next[i] = 0;

        /* transition */
        for (int p = 0; p < 9; ++p) {             /* previous column: 0..7 or dummy 8 */
            for (int c = 0; c < 8; ++c) {          /* current column */
                seq_t cnt = dp_prev[p * 8 + c];
                if (!cnt)
                    continue;
                for (int ncol = 0; ncol < 8; ++ncol) {
                    if (ncol == 7) /* forbid vertical 111 */
                        continue;
                    if (!valid_triplet(p, c, ncol))
                        continue;
                    int idx = c * 8 + ncol;       /* new state (prev=c, curr=ncol) */
                    dp_next[idx] += cnt;
                }
            }
        }
        /* copy next to prev for next iteration */
        for (int i = 0; i < STATES; ++i)
            dp_prev[i] = dp_next[i];

        /* total for this length */
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i)
            total += dp_prev[i];

        grow_cache(len);
        cache[len - 1] = total;
    }
    cache_len = N;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0 */
    compute_up_to((size_t)n);
    return cache[n - 1];
}
