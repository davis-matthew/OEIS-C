/*
 * A207844  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A207844
 *  Number of n×7 binary arrays avoiding horizontal patterns
 *      000 and 001,
 *  and vertical patterns
 *      000 and 111.
 *
 *  Computed by dynamic programming over rows.
 *  Two consecutive rows form the state; transitions enforce
 *  the vertical constraint on triples of consecutive rows.
 *
 *  The recurrence is implicit:
 *      a(1) = 42                            (the number of valid rows)
 *      a(2) = 42²
 *      for n ≥ 3:
 *          a(n) = sum over all states of DP[n][state]
 *
 *  The code is memoised: the transition table and DP buffers are
 *  built on the first call and reused for later calls, independent
 *  of call order.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

#define MAX_BITS    7
#define MAX_ROWS   (1 << MAX_BITS)    /* 128 */
#define MAX_R      MAX_ROWS           /* at most 128 valid rows */
#define MAX_S      (MAX_R * MAX_R)    /* at most 16384 states */

/* ---- Helpers for row validity ------------------------------------ */
static inline bool horizontal_ok(uint8_t row)
{
    /* Reject any 3‑bit window 000 or 001. */
    for (int j = 0; j <= 4; ++j) {
        uint8_t tri = (row >> j) & 7u;
        if (tri == 0u || tri == 1u)
            return false;
    }
    return true;
}

static inline bool vertical_ok(uint8_t r0, uint8_t r1, uint8_t r2)
{
    /* In any column the three bits cannot all be 0 or all be 1. */
    uint8_t all1 = r0 & r1 & r2;
    uint8_t all0 = (~r0) & (~r1) & (~r2) & 0x7Fu;
    return (all1 | all0) == 0;
}

/* ---- Transition table -------------------------------------------- */
typedef struct {
    uint16_t cnt;              /* number of outgoing transitions */
    uint16_t next[MAX_R];      /* indices of resulting states */
} Trans;

/* ---- Static storage ----------------------------------------------- */
static uint16_t          valid_rows[MAX_R];
static uint16_t          Rsize = 0;               /* number of valid rows */
static Trans            trans[MAX_S];             /* transition table */

static seq_t *dp = NULL;          /* DP array for current row */
static seq_t *ndp = NULL;         /* DP array for next row */
static size_t  dp_size = 0;       /* allocated size (S) */

/* ---- Initialisation ------------------------------------------------ */
static void init_tables(void)
{
    if (Rsize != 0)   /* already initialised */
        return;

    /* 1. enumerate all horizontally valid rows */
    for (uint16_t r = 0; r < MAX_ROWS; ++r) {
        if (horizontal_ok((uint8_t)r))
            valid_rows[Rsize++] = r;
    }

    /* 2. build transition table over states (prev2, prev1) */
    for (uint16_t i = 0; i < Rsize; ++i) {
        uint8_t r0 = valid_rows[i];
        for (uint16_t j = 0; j < Rsize; ++j) {
            uint8_t r1 = valid_rows[j];
            uint16_t state = i * Rsize + j;
            Trans *t = &trans[state];
            t->cnt = 0;
            for (uint16_t k = 0; k < Rsize; ++k) {
                uint8_t r2 = valid_rows[k];
                if (vertical_ok(r0, r1, r2)) {
                    t->next[t->cnt++] = j * Rsize + k;   /* new state (prev1, curr) */
                }
            }
        }
    }

    /* 3. allocate DP buffers (size depends on Rsize) */
    size_t S = (size_t)Rsize * (size_t)Rsize;
    dp = seq_alloc(S);
    ndp = seq_alloc(S);
    dp_size = S;
}

/* ---- Main function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* sequence is undefined for n < 1 */

    init_tables();

    const uint16_t R = Rsize;
    const size_t   S = (size_t)R * (size_t)R;

    /* Handle the trivial small n directly */
    if (n == 1) return (seq_t)R;
    if (n == 2) return (seq_t)R * (seq_t)R;

    /* Initialise DP for the base case (first two rows) */
    for (size_t s = 0; s < S; ++s)
        dp[s] = (seq_t)1;

    /* Process rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero next DP buffer */
        for (size_t s = 0; s < S; ++s)
            ndp[s] = (seq_t)0;

        /* propagate through all transitions */
        for (size_t s = 0; s < S; ++s) {
            seq_t cnt = dp[s];
            if (cnt == (seq_t)0) continue;
            Trans *t = &trans[s];
            for (uint16_t t_idx = 0; t_idx < t->cnt; ++t_idx) {
                size_t ns = (size_t)t->next[t_idx];
                ndp[ns] += cnt;
            }
        }

        /* swap dp and ndp */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* Sum all final state counts to obtain the total number of arrays */
    seq_t total = (seq_t)0;
    for (size_t s = 0; s < S; ++s)
        total += dp[s];

    return total;
}
