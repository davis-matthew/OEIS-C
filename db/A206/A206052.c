/*
 * A206052  Number of (n+1)X7 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one.
 * https://oeis.org/A206052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 0..2 arrays with exactly one clockwise
 *            edge increase in every 2×2 subblock
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 19,
 *            big any n
 * Helpers:   clear_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/clear_seq_array.h"

/* ------------------------------------------------------------------ */
/*  Macros for allocation that work in both C and C++                 */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
#  define ALLOC(type, n) new type[(n)]
#  define FREE(ptr)      delete [] (ptr)
#else
#  define ALLOC(type, n) ((type*)calloc((n), sizeof(type)))
#  define FREE(ptr)      free(ptr)
#endif

/* ------------------------------------------------------------------ */
/*  Constants                                                        */
/* ------------------------------------------------------------------ */
#define STATE_DIGITS 2187            /* 3^7 */
#define COLS          7

/* ------------------------------------------------------------------ */
/*  Helper functions                                                 */

/* ------------------------------------------------------------------ */
/*  Global data                                                      */
/* ------------------------------------------------------------------ */
static uint8_t *state_digits = NULL;          /* STATE_DIGITS * COLS bytes */
static size_t  *adj_start   = NULL;           /* STATE_DIGITS + 1 entries */
static uint16_t *adj_next    = NULL;           /* total edges entries */
static seq_t   *curr_counts = NULL;           /* STATE_DIGITS entries */
static seq_t   *next_counts = NULL;           /* STATE_DIGITS entries */

static seq_t  *memo_vals   = NULL;            /* memoized a(n) values */
static size_t  memo_cap    = 0;               /* current capacity of memo_vals */
static idx_t   computed    = 0;               /* highest n for which memo is filled */

/* ------------------------------------------------------------------ */
/*  Transition test: does pair of rows (a -> b) satisfy the rule?   */
/* ------------------------------------------------------------------ */
static bool ok_transition(const uint8_t *a, const uint8_t *b)
{
    for (size_t k = 0; k < COLS - 1; ++k) {
        bool inc1 = a[k]     < a[k + 1];
        bool inc2 = a[k + 1] < b[k + 1];
        bool inc3 = b[k + 1] < b[k];
        bool inc4 = b[k]     < a[k];
        int sum = inc1 + inc2 + inc3 + inc4;
        if (sum != 1) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  One‑time initialization of all tables                            */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    /* 1. Allocate memory for state digits and build them               */
    state_digits = ALLOC(uint8_t, STATE_DIGITS * COLS);
    for (size_t i = 0; i < STATE_DIGITS; ++i) {
        int t = (int)i;
        for (int k = COLS - 1; k >= 0; --k) {
            state_digits[i * COLS + k] = t % 3;
            t /= 3;
        }
    }

    /* 2. Compute out‑degrees (number of admissible successors)       */
    size_t *degree = ALLOC(size_t, STATE_DIGITS);
    for (size_t i = 0; i < STATE_DIGITS; ++i) {
        const uint8_t *a = state_digits + i * COLS;
        size_t cnt = 0;
        for (size_t j = 0; j < STATE_DIGITS; ++j) {
            const uint8_t *b = state_digits + j * COLS;
            if (ok_transition(a, b)) ++cnt;
        }
        degree[i] = cnt;
    }

    /* 3. Allocate adjacency buffers                                   */
    size_t total_edges = 0;
    for (size_t i = 0; i < STATE_DIGITS; ++i) total_edges += degree[i];
    adj_start = ALLOC(size_t, STATE_DIGITS + 1);
    adj_next  = ALLOC(uint16_t, total_edges);

    /* 4. Fill adjacency lists                                         */
    size_t offset = 0;
    for (size_t i = 0; i < STATE_DIGITS; ++i) {
        adj_start[i] = offset;
        const uint8_t *a = state_digits + i * COLS;
        for (size_t j = 0; j < STATE_DIGITS; ++j) {
            const uint8_t *b = state_digits + j * COLS;
            if (ok_transition(a, b)) adj_next[offset++] = (uint16_t)j;
        }
    }
    adj_start[STATE_DIGITS] = offset;            /* sentinel */

    FREE(degree);                                 /* temporary array */

    /* 5. Allocate DP arrays                                            */
    curr_counts = ALLOC(seq_t, STATE_DIGITS);
    next_counts = ALLOC(seq_t, STATE_DIGITS);
    for (size_t i = 0; i < STATE_DIGITS; ++i) curr_counts[i] = 1;   /* one‑row arrays */
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 1. Make sure tables are ready                                     */
    static bool tables_inited = false;
    if (!tables_inited) {
        init_tables();
        tables_inited = true;
    }

    /* 2. Expand memoization array if necessary                        */
    if (n >= memo_cap) {
        size_t new_cap = memo_cap ? memo_cap * 2 : 32;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *new_arr;
#ifdef __cplusplus
        new_arr = new seq_t[new_cap];
        for (size_t i = memo_cap; i < new_cap; ++i) new_arr[i] = 0;
        if (memo_vals) {
            for (size_t i = 0; i < memo_cap; ++i) new_arr[i] = memo_vals[i];
            delete [] memo_vals;
        }
#else
        new_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (memo_vals) {
            memcpy(new_arr, memo_vals, memo_cap * sizeof(seq_t));
            free(memo_vals);
        }
#endif
        memo_vals = new_arr;
        memo_cap  = new_cap;
    }

    /* 3. Compute up to required n                                      */
    while (computed < n) {
        /* zero next_counts */
        clear_seq_array(next_counts, STATE_DIGITS);

        /* transition step */
        for (size_t i = 0; i < STATE_DIGITS; ++i) {
            seq_t ci = curr_counts[i];
            if (ci == 0) continue;
            for (size_t pi = adj_start[i]; pi < adj_start[i + 1]; ++pi) {
                size_t j = adj_next[pi];
                next_counts[j] += ci;
            }
        }

        /* sum all counts → a(computed + 1) */
        seq_t sum = 0;
        for (size_t i = 0; i < STATE_DIGITS; ++i) sum += next_counts[i];
        memo_vals[computed + 1] = sum;

        /* prepare for next step */
        seq_t *tmp = curr_counts;
        curr_counts = next_counts;
        next_counts = tmp;
        ++computed;
    }

    /* 4. Return memoized result                                          */
    return memo_vals[n];
}
