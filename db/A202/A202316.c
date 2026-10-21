/*
 * A202316  Number of (n+2)X9 binary arrays avoiding patterns 001 and 100 in
 *          rows and columns.
 * https://oeis.org/A202316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed via a transfer matrix over the 9‑bit row
 *            patterns that avoid the horizontal patterns 001 and 100. For
 *            each such row pattern a[ ], b[ ], c[ ] (length 9) we check the
 *            vertical constraint that no column contains 001 or 100 in three
 *            consecutive rows. The dynamic programme counts how...
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* constants for 9‑bit patterns */
#define MAX_ROWS 512          /* 2^9 */
#define MASK9   0x1FF         /* mask for 9 bits */

/* runtime data (initialized on first call) */
static bool initialized = false;
static size_t S = 0;               /* number of legal row patterns */
static uint16_t *row_vals = NULL;  /* legal row patterns */
static size_t *nr_list_len = NULL; /* number of next rows for each pair */
static uint16_t **next_lists = NULL;/* list of next rows for each pair */
static seq_t *dp = NULL;           /* DP table for current step       */
static seq_t *dp_next = NULL;      /* DP table for next step           */

/* ------------------------------------------------------------------ */
/* test whether a 9‑bit pattern contains 001 or 100 horizontally */
static bool row_ok(uint16_t r)
{
    for (int j = 0; j <= 6; ++j) {
        uint8_t pat = (r >> j) & 7;
        if (pat == 1 || pat == 4)   /* 001 or 100 */
            return false;
    }
    return true;
}

/* test whether three rows avoid pattern 001 or 100 vertically */
static bool vert_ok(uint16_t a, uint16_t b, uint16_t c)
{
    uint16_t inv_a = (~a) & MASK9;
    uint16_t inv_b = (~b) & MASK9;
    uint16_t inv_c = (~c) & MASK9;
    uint16_t m1 = inv_a & inv_b & c;   /* 001 */
    uint16_t m2 = a & inv_b & inv_c;   /* 100 */
    return (m1 | m2) == 0;
}

/* ------------------------------------------------------------------ */
/* initialise static tables once */
static void init_sequence(void)
{
    /* 1. enumerate legal row patterns */
    uint16_t allr[MAX_ROWS];
    size_t cnt = 0;
    for (uint16_t r = 0; r < MAX_ROWS; ++r)
        if (row_ok(r))
            allr[cnt++] = r;
    S = cnt;
    row_vals = (uint16_t *)malloc(S * sizeof(uint16_t));
    for (size_t i = 0; i < S; ++i)
        row_vals[i] = allr[i];

    /* 2. build adjacency lists: for each pair (a,b) list of c */
    size_t pairs = S * S;
    nr_list_len = (size_t *)malloc(pairs * sizeof(size_t));
    next_lists = (uint16_t **)malloc(pairs * sizeof(uint16_t *));
    /* first pass: count lengths */
    for (size_t ai = 0; ai < S; ++ai) {
        uint16_t a = row_vals[ai];
        for (size_t bi = 0; bi < S; ++bi) {
            uint16_t b = row_vals[bi];
            size_t idx = ai * S + bi;
            size_t ccount = 0;
            for (size_t ci = 0; ci < S; ++ci) {
                uint16_t c = row_vals[ci];
                if (vert_ok(a, b, c))
                    ++ccount;
            }
            nr_list_len[idx] = ccount;
            if (ccount > 0)
                next_lists[idx] = (uint16_t *)malloc(ccount * sizeof(uint16_t));
            else
                next_lists[idx] = NULL;
        }
    }
    /* second pass: fill lists */
    for (size_t ai = 0; ai < S; ++ai) {
        uint16_t a = row_vals[ai];
        for (size_t bi = 0; bi < S; ++bi) {
            uint16_t b = row_vals[bi];
            size_t idx = ai * S + bi;
            size_t pos = 0;
            for (size_t ci = 0; ci < S; ++ci) {
                uint16_t c = row_vals[ci];
                if (vert_ok(a, b, c))
                    next_lists[idx][pos++] = (uint16_t)ci;
            }
        }
    }

    /* 3. allocate DP tables and zero them */
    dp = (seq_t *)malloc(pairs * sizeof(seq_t));
    dp_next = (seq_t *)malloc(pairs * sizeof(seq_t));
    for (size_t i = 0; i < pairs; ++i) {
        dp[i] = (seq_t)0;
        dp_next[i] = (seq_t)0;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* defined only for n >= 1 */

    if (!initialized)
        init_sequence();

    /* number of legal rows pairs (row1,row2) */
    size_t pairs = S * S;

    /* initialise DP: all pairs are possible with count 1 */
    for (size_t i = 0; i < pairs; ++i)
        dp[i] = (seq_t)1;

    /* perform n transitions to add the remaining n rows (rows 2..n+1) */
    for (idx_t step = 0; step < n; ++step) {
        /* zero dp_next */
        for (size_t i = 0; i < pairs; ++i)
            dp_next[i] = (seq_t)0;

        /* iterate over all current state pairs (a,b) */
        for (size_t ai = 0; ai < S; ++ai) {
            for (size_t bi = 0; bi < S; ++bi) {
                size_t idx = ai * S + bi;
                seq_t val = dp[idx];
                if (val == (seq_t)0)
                    continue;

                size_t len = nr_list_len[idx];
                uint16_t *list = next_lists[idx];
                for (size_t k = 0; k < len; ++k) {
                    uint16_t ci = list[k];
                    size_t nidx = bi * S + ci;   /* new state (b,c) */
                    dp_next[nidx] += val;
                }
            }
        }

        /* swap dp tables */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* sum over all final states */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < pairs; ++i)
        total += dp[i];

    return total;
}
