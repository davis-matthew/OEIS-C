/*
 * A234069  Number of (1+2)X(n+2) 0..3 arrays with no increasing sequence of
 *          length 3 horizontally or diagonally downwards.
 * https://oeis.org/A234069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute a(n) as the number of 0..3 arrays of size 3 × (n+2) that
 *            contain no increasing sequence of length 3 horizontally or along
 *            the main diagonal. The implementation uses a transfer‑matrix
 *            dynamic programming over the columns
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   col_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/col_val.h"

/* Auxiliary: number of bits required to store a 64‑bit integer. */
#ifndef UINT64_MAX
#define UINT64_MAX ((uint64_t)-1)
#endif

/*-------------------------------------------------------------------------*/
/*  Serialization of a column (3 rows) 0..3 into a 6‑bit value.             */

/*-------------------------------------------------------------------------*/
/*  Pre‑computed transition table.
     For each pair index p = s1*64 + s2 (prev two columns) we store
     all possible indices q = s2*64 + s3 of the next pair that satisfy
     the constraints.  */
#define STATE_COUNT 64            /* 4^3 possible columns                           */
#define PAIR_COUNT  (STATE_COUNT * STATE_COUNT)  /* 4096 pairs           */
#define MAXDEG      64

static bool precomp_done = false;
static uint8_t  trans_len[PAIR_COUNT];
static uint16_t trans_next[PAIR_COUNT][MAXDEG];

/*-----------------------------------------------------------------------*/
/*  Initialization: build the transition table.                           */
static void init_transitions(void)
{
    if (precomp_done) return;
    for (uint16_t s1 = 0; s1 < STATE_COUNT; ++s1) {
        for (uint16_t s2 = 0; s2 < STATE_COUNT; ++s2) {
            uint16_t p = s1 * STATE_COUNT + s2;
            uint8_t deg = 0;
            for (uint16_t s3 = 0; s3 < STATE_COUNT; ++s3) {
                bool bad = false;
                /* horizontal checks for each row */
                for (uint8_t r = 0; r < 3 && !bad; ++r) {
                    uint8_t v1 = col_val(r, s1);
                    uint8_t v2 = col_val(r, s2);
                    uint8_t v3 = col_val(r, s3);
                    if (v1 < v2 && v2 < v3) bad = true;
                }
                /* diagonal check (top->mid->bot) */
                if (!bad) {
                    uint8_t t = col_val(0, s1);
                    uint8_t m = col_val(1, s2);
                    uint8_t b = col_val(2, s3);
                    if (t < m && m < b) bad = true;
                }
                if (!bad) {
                    uint16_t q = s2 * STATE_COUNT + s3;
                    trans_next[p][deg++] = q;
                }
            }
            trans_len[p] = deg;
        }
    }
    precomp_done = true;
}

/*-------------------------------------------------------------------------*/
/*  Main sequence function                                             */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_transitions();

    /* static cache for computed values */
    static seq_t *cache = NULL;     /* cache[i] holds a(i+1) */
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;     /* processed up to cache_max */

    /* static state vectors for DP */
    static seq_t cur[PAIR_COUNT];
    static seq_t next[PAIR_COUNT];

    /* initialize DP for the first two columns */
    static bool dp_init = false;
    if (!dp_init) {
        for (uint16_t i = 0; i < PAIR_COUNT; ++i) cur[i] = (seq_t)1;
        memset(next, 0, sizeof next);
        dp_init = true;
        cache_max = 0;
    }

    /* extend cache up to requested n */
    while (cache_max < n) {
        /* one DP step: add a new column */
        memset(next, 0, sizeof next);
        for (uint16_t p = 0; p < PAIR_COUNT; ++p) {
            seq_t val = cur[p];
            if (val == 0) continue;
            uint8_t deg = trans_len[p];
            uint16_t *t = trans_next[p];
            for (uint8_t k = 0; k < deg; ++k) {
                next[t[k]] += val;
            }
        }
        /* copy next to cur (now cur holds state after width+1 columns) */
        memcpy(cur, next, sizeof cur);

        /* compute total for this n */
        seq_t total = 0;
        for (uint16_t i = 0; i < PAIR_COUNT; ++i) total += cur[i];

        /* store in cache */
        if (cache_max + 1 > cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            while (new_cap <= cache_max + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_max] = total;
        ++cache_max;
    }

    return cache[n - 1];
}
