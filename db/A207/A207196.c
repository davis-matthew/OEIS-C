/*
 * A207196  Number of nX3 0..2 arrays avoiding the pattern z-1 z-1 z in any
 *          row or column.
 * https://oeis.org/A207196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   realloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207196: Number of n×3 arrays over {0,1,2} avoiding the pattern
 *  z-1 z-1 z in any row or column.
 *  Computation via dynamic programming with a 625‑state automaton.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/realloc_seq_array.h"

#define STATE_ROWS 25               /* 27 total rows minus {1,14} impossible */
#define STATE_COUNT (STATE_ROWS * STATE_ROWS)

/* Legal rows (0..26) except 1 and 14 */
static uint8_t legal_rows[STATE_ROWS];
static int8_t  legal_index[27];
static uint8_t row_digit[STATE_ROWS][3];

/* State IDs: pair of consecutive rows */
static uint8_t lr1[STATE_COUNT];
static uint8_t lr2[STATE_COUNT];

/* Transition tables */
static uint8_t trans[STATE_COUNT][STATE_ROWS];
static uint8_t trans_len[STATE_COUNT];

/* DP tables for counting arrays of current and next length */
static seq_t *dp_curr = NULL;
static seq_t *dp_next = NULL;

/* Cached answers a(1), a(2), … up to cached_upto */
static seq_t *ans_cache = NULL;
static size_t cache_size = 0;      /* number of elements stored */
static idx_t cached_upto = 0;

/* Helper: allocate an array of seq_t objects */
static seq_t *alloc_seq_array(size_t n) {
#if defined(__cplusplus) && __cplusplus >= 201103L
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Ensure the answer cache can hold values up to n (1‑based). */
static void ensure_cache(idx_t n) {
    size_t target = (size_t)n;           /* we store index n at position n‑1 */
    if (target > cache_size) {
        seq_t *newc = realloc_seq_array(ans_cache, cache_size, target);
        if (!newc) { exit(1); }
        ans_cache = newc;
        cache_size = target;
    }
}

/* Initialisation of automaton tables and DP arrays.  Called once. */
static void init_tables(void) {
    /* Build list of legal rows and per-row digits */
    for (int r = 0; r < 27; ++r) legal_index[r] = -1;
    int idx = 0;
    for (int r = 0; r < 27; ++r) {
        if (r == 1 || r == 14) continue;          /* forbidden */
        legal_rows[idx] = (uint8_t)r;
        legal_index[r] = (int8_t)idx;
        row_digit[idx][0] = r % 3;
        row_digit[idx][1] = (r / 3) % 3;
        row_digit[idx][2] = (r / 9) % 3;
        ++idx;
    }

    /* Enumerate all state pairs and compute transitions */
    for (int s = 0; s < STATE_COUNT; ++s) {
        lr1[s] = s / STATE_ROWS;
        lr2[s] = s % STATE_ROWS;
        uint8_t len = 0;
        uint8_t r1 = legal_rows[lr1[s]];
        uint8_t r2 = legal_rows[lr2[s]];
        for (int lr3 = 0; lr3 < STATE_ROWS; ++lr3) {
            uint8_t r3 = legal_rows[lr3];
            bool ok = true;
            for (int c = 0; c < 3 && ok; ++c) {
                uint8_t a = row_digit[lr1[s]][c];
                uint8_t b = row_digit[lr2[s]][c];
                uint8_t cc = row_digit[lr3][c];
                if (a == b && b + 1 == cc) ok = false;
            }
            if (ok) trans[s][len++] = (uint8_t)lr3;
        }
        trans_len[s] = len;
    }

    /* DP tables: start with length 2 */
    dp_curr = alloc_seq_array(STATE_COUNT);
    dp_next = alloc_seq_array(STATE_COUNT);
    for (size_t i = 0; i < STATE_COUNT; ++i) dp_curr[i] = 1;

    /* Cache initial known values a(1)=25, a(2)=625 */
    ans_cache = alloc_seq_array(2);
    ans_cache[0] = 25;
    ans_cache[1] = 625;
    cache_size = 2;
    cached_upto = 2;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n=0 */
    if (!dp_curr) init_tables();   /* initialise on first call */

    if (n <= cached_upto) return ans_cache[n - 1];

    for (idx_t len = cached_upto + 1; len <= n; ++len) {
        /* zero next DP array */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* transitions from current states */
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_curr[s];
            if (!val) continue;
            uint8_t l2 = lr2[s];
            uint8_t tlen = trans_len[s];
            for (uint8_t k = 0; k < tlen; ++k) {
                uint8_t lr3 = trans[s][k];
                size_t dst = (size_t)l2 * STATE_ROWS + lr3;
                dp_next[dst] += val;
            }
        }

        /* sum all states for total arrays of this length */
        seq_t total = 0;
        for (size_t i = 0; i < STATE_COUNT; ++i) total += dp_next[i];

        /* store result and prepare for next iteration */
        ensure_cache(len);
        ans_cache[len - 1] = total;
        seq_t *tmp = dp_curr; dp_curr = dp_next; dp_next = tmp;
    }

    cached_upto = n;
    return ans_cache[n - 1];
}
