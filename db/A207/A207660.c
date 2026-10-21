/*
 * A207660  Number of nX7 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207660: Number of n×7 binary arrays avoiding
   horizontal 000 and 111 and vertical 001 and 100.
   Computation is done by a transfer‑matrix DP on the last two rows. */

#include <stdint.h>
#include <stdbool.h>

#define MAX_BITS 128           /* 2^7 possible rows */
#define MAX_ROWS 128           /* maximum unique rows (≤128) */
#define MAX_S    (MAX_ROWS*MAX_ROWS)   /* maximum number of state pairs */

static idx_t       M = 0;                 /* admissible row count (≤42) */
static idx_t       S = 0;                 /* M*M */
static uint8_t     rows[MAX_ROWS];        /* masks of admissible rows */
static uint8_t     succIdx[MAX_S][MAX_ROWS];
static uint8_t     succCnt[MAX_S];
static bool        succInit = false;

/* Generate all 7‑bit rows that avoid 000 and 111 horizontally. */
static void init_rows(void)
{
    M = 0;
    for (idx_t mask = 0; mask < MAX_BITS; ++mask) {
        bool ok = true;
        for (idx_t p = 0; p < 5; ++p) {            /* 5 windows of 3 bits in 7 bits */
            uint8_t w = (mask >> p) & 7U;
            if (w == 0 || w == 7) { ok = false; break; }
        }
        if (ok) rows[M++] = (uint8_t)mask;
    }
    S = M * M;
}

/* Precompute for every pair (a,b) the list of c such that rows a,b,c
   satisfy the vertical 001/100 restriction. */
static void init_successors(void)
{
    if (succInit) return;
    init_rows();

    for (idx_t s = 0; s < S; ++s) succCnt[s] = 0;

    for (idx_t a = 0; a < M; ++a) {
        for (idx_t b = 0; b < M; ++b) {
            idx_t s = a * M + b;
            idx_t count = 0;
            for (idx_t c = 0; c < M; ++c) {
                bool ok = true;
                for (idx_t p = 0; p < 7; ++p) {
                    uint8_t bit_a = (rows[a] >> p) & 1U;
                    uint8_t bit_b = (rows[b] >> p) & 1U;
                    uint8_t bit_c = (rows[c] >> p) & 1U;
                    if ((bit_a == 0 && bit_b == 0 && bit_c == 1) ||
                        (bit_a == 1 && bit_b == 0 && bit_c == 0)) {
                        ok = false; break;
                    }
                }
                if (ok) {
                    succIdx[s][count++] = (uint8_t)c;
                }
            }
            succCnt[s] = (uint8_t)count;
        }
    }
    succInit = true;
}

/* ------------------------------ main sequence ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    init_successors();             /* ensure M, S and successor tables are ready */

    if (n == 1) return (seq_t)M;                     /* single row */
    if (n == 2) return (seq_t)M * (seq_t)M;           /* two rows, no vertical 3‑tuple */

    static seq_t dp[MAX_S];
    static seq_t dpNext[MAX_S];

    /* initialise dp for step 2 (all pairs counted once) */
    for (idx_t s = 0; s < S; ++s) dp[s] = (seq_t)1;

    /* extend step by step until length n is reached */
    for (idx_t step = 3; step <= n; ++step) {
        for (idx_t s = 0; s < S; ++s) dpNext[s] = 0;
        for (idx_t s = 0; s < S; ++s) {
            seq_t ways = dp[s];
            if (!ways) continue;
            idx_t a = s / M;          /* previous row */
            idx_t b = s % M;          /* row before last */
            idx_t nb   = succCnt[s];
            for (idx_t k = 0; k < nb; ++k) {
                idx_t c       = succIdx[s][k];
                idx_t nextIdx = b * M + c;
                dpNext[nextIdx] += ways;
            }
        }
        /* swap dp and dpNext */
        for (idx_t s = 0; s < S; ++s) dp[s] = dpNext[s];
    }

    /* sum all states to obtain total count for length n */
    seq_t total = 0;
    for (idx_t s = 0; s < S; ++s) total += dp[s];
    return total;
}
