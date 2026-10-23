/*
 * A185848  Number of (n+1) X 2 0..2 arrays with no 2 X 2 subblock diagonal
 *          sum less antidiagonal sum equal to any horizontal or vertical
 *          neighbor 2 X 2 subblock diagonal sum less antidiagonal sum.
 * https://oeis.org/A185848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 20*a(n-2) + 99*a(n-3) + 252*a(n-4) + *
 *            510*a(n-5) + 324*a(n-6) – 336*a(n-7) – 288*a(n-8) * a(1) = 81,
 *            a(2) = 630, … (first 18 values in OEIS entry) * The sequence
 *            counts (n+1)×2 arrays with entries 0..2 where every * vertical
 *            2×2 subblock has a diagonal–antidiagonal sum differen...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for malloc, realloc, exit */
#include <stdint.h> /* for uintptr_t if needed */

#define ROWS        9          /* 0..8  (three levels of a and three of b) */
#define D_RANGE     9          /* difference values –4..+4 */
#define STATE_COUNT (ROWS * D_RANGE)  /* 81 distinct states */

/* all 9 possible rows (a,b) with a,b ∈ {0,1,2} */
static const int row_vals[ROWS][2] = {
    {0,0}, {0,1}, {0,2},
    {1,0}, {1,1}, {1,2},
    {2,0}, {2,1}, {2,2}
};

/* DP storage for the current and next step */
static seq_t cur_state[STATE_COUNT];
static seq_t nxt_state[STATE_COUNT];

/* cached sequence values a(0..N) */
static seq_t *cache   = NULL;   /* dynamic array */
static size_t cache_len = 0;    /* size of cache array, initially 0 */
static idx_t  max_n_computed = 0;   /* largest n for which cache[n] is filled */

/* allocate or extend the cache array to accommodate n (≥0) */
static void ensure_cache_size(idx_t n)
{
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* out‑of‑memory – abort silently */
            exit(1);
        }
        /* zero the new entries */
        for (size_t i = cache_len; i < new_len; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_len = new_len;
    }
}

/* compute a(n) for n≥1; a(0) is unused and returns 0 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* guard against very large n that would overflow cache length */
    if (n > (idx_t)(cache_len - 1))
        ensure_cache_size(n);

    /* if already computed, return cached value */
    if (max_n_computed >= n)
        return cache[n];

    /* ----- initialisation for n=1 (first subblock) ----- */
    if (max_n_computed == 0) {
        /* zero current state */
        for (int s = 0; s < STATE_COUNT; ++s)
            cur_state[s] = 0;

        /* process all pairs of rows (row0,row1) */
        for (int r0 = 0; r0 < ROWS; ++r0) {
            for (int r1 = 0; r1 < ROWS; ++r1) {
                int d = row_vals[r0][0] + row_vals[r1][1]
                        - row_vals[r0][1] - row_vals[r1][0];   /* –4 … +4 */
                int state = r1 * D_RANGE + (d + 4);          /* state index */
                cur_state[state] += (seq_t)1;
            }
        }

        /* total for n=1 */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s)
            total += cur_state[s];

        ensure_cache_size(1);
        cache[1] = total;
        max_n_computed = 1;
    }

    /* ----- extend computation up to requested n ----- */
    while (max_n_computed < n) {
        /* zero next state */
        for (int s = 0; s < STATE_COUNT; ++s)
            nxt_state[s] = 0;

        /* transition from current to next row */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = cur_state[s];
            if (cnt == (seq_t)0) continue;
            int prev_row = s / D_RANGE;           /* 0..8 */
            int prev_d   = (s % D_RANGE) - 4;     /* –4..+4 */

            for (int next_row = 0; next_row < ROWS; ++next_row) {
                int cur_d = row_vals[prev_row][0] + row_vals[next_row][1]
                            - row_vals[prev_row][1] - row_vals[next_row][0];
                if (cur_d == prev_d) continue;          /* cannot equal previous d */

                int new_state = next_row * D_RANGE + (cur_d + 4);
                nxt_state[new_state] += cnt;
            }
        }

        /* swap cur and nxt */
        for (int s = 0; s < STATE_COUNT; ++s)
            cur_state[s] = nxt_state[s];

        /* compute total for this n */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s)
            total += cur_state[s];

        ensure_cache_size((idx_t)(max_n_computed + 1));
        cache[++max_n_computed] = total;
    }

    return cache[n];
}
