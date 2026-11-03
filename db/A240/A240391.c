/*
 * A240391  Number of nX4 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or two plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries 0..3 satisfying the
 *            constraint that for each element e at position (i,j) e ≠ (0 +
 *            sum of elements to its left) mod 4, e ≠ (1 + sum of elements
 *            above it) mod 4, e ≠ (2 + sum of elements diagonally to its
 *            northwest) mod 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 194 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline void dec_state(uint16_t s, uint8_t up[4], uint8_t diag[4]) {
    up[0]   =  s       & 3;
    up[1]   = (s >>  2) & 3;
    up[2]   = (s >>  4) & 3;
    up[3]   = (s >>  6) & 3;
    diag[0] = (s >>  8) & 3;
    diag[1] = (s >> 10) & 3;
    diag[2] = (s >> 12) & 3;
    diag[3] = (s >> 14) & 3;
}

/*  16‑bit state: 4 bits for the four column sums (upsum),
                    4 bits for the four diagonal sums (diag), each 2 bits (0..3).   */
static inline uint16_t enc_state(const uint8_t up[4], const uint8_t diag[4]) {
    uint16_t s = 0;
    s |=  up[0] & 3;
    s |= (up[1] & 3) << 2;
    s |= (up[2] & 3) << 4;
    s |= (up[3] & 3) << 6;
    s |= (diag[0] & 3) << 8;
    s |= (diag[1] & 3) <<10;
    s |= (diag[2] & 3) <<12;
    s |= (diag[3] & 3) <<14;
    return s;
}

/* ------------------------------------------------------------------ */
/* State representation ------------------------------------------------*/

/* ------------------------------------------------------------------ */
/* Transition tables ----------------------------------------------------*/
static uint32_t *trans_start = NULL;      /* size 65537 */
static uint16_t *trans_next  = NULL;     /* cumulative edge list */

/* One‑time transition precomputation */
static void make_transitions(void) {
    const int STATES = 65536;
    uint32_t *edge_cnt = (uint32_t*)malloc(sizeof(uint32_t)*STATES);
    if (!edge_cnt) abort();

    /* first pass – count edges for every state */
    for (int s = 0; s < STATES; ++s) {
        uint8_t up[4], diag[4];
        dec_state((uint16_t)s, up, diag);
        uint32_t cnt = 0;
        for (int row = 0; row < 256; ++row) {
            uint8_t left = 0;
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                uint8_t a = (row >> (j*2)) & 3;
                uint8_t f0 = left & 3;
                uint8_t f1 = (1 + up[j]) & 3;
                uint8_t f2 = (2 + diag[j]) & 3;
                if (a == f0 || a == f1 || a == f2) { ok = false; break; }
                left = (left + a) & 3;
            }
            if (!ok) continue;
            ++cnt;
        }
        edge_cnt[s] = cnt;
    }

    /* build cumulative start indices */
    trans_start = (uint32_t*)malloc(sizeof(uint32_t)*(STATES+1));
    trans_start[0] = 0;
    for (int s = 0; s < STATES; ++s)
        trans_start[s+1] = trans_start[s] + edge_cnt[s];

    uint32_t total_edges = trans_start[STATES];
    trans_next = (uint16_t*)malloc(sizeof(uint16_t)*total_edges);
    if (!trans_start || !trans_next) abort();

    /* second pass – fill edges */
    for (int s = 0; s < STATES; ++s) {
        uint8_t up[4], diag[4];
        dec_state((uint16_t)s, up, diag);
        uint32_t idx = trans_start[s];
        for (int row = 0; row < 256; ++row) {
            uint8_t left = 0;
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                uint8_t a = (row >> (j*2)) & 3;
                uint8_t f0 = left & 3;
                uint8_t f1 = (1 + up[j]) & 3;
                uint8_t f2 = (2 + diag[j]) & 3;
                if (a == f0 || a == f1 || a == f2) { ok = false; break; }
                left = (left + a) & 3;
            }
            if (!ok) continue;
            /* compute new up and diag after this row */
            uint8_t up2[4], diag2[4];
            for (int j = 0; j < 4; ++j) {
                uint8_t a = (row >> (j*2)) & 3;
                up2[j]   = (up[j] + a) & 3;
                diag2[j] = (diag[j] + a) & 3;
            }
            /* shift diagonal sums for next row */
            uint8_t nd[4];
            nd[0] = 0;
            nd[1] = diag2[0];
            nd[2] = diag2[1];
            nd[3] = diag2[2];
            uint16_t ns = enc_state(up2, nd);
            trans_next[idx++] = ns;
        }
    }
    free(edge_cnt);
}

/* ------------------------------------------------------------------ */
/* Main sequence function --------------------------------------------*/
seq_t a(idx_t n) {
    static bool ready = false;
    if (!ready) { make_transitions(); ready = true; }

    /* DP containers */
    const size_t N = 65536;          /* number of states */
    static seq_t *dp  = NULL;
    static seq_t *nxt = NULL;
    if (!dp) {
        dp  = mem_new(seq_t, N);
        nxt = mem_new(seq_t, N);
    }

    /* Sets of active states for each step */
    static int  *act  = NULL;
    static int  *nact = NULL;
    static bool *seen = NULL;
    if (!act) {
        act  = (int*)malloc(sizeof(int)*N);
        nact = (int*)malloc(sizeof(int)*N);
        seen = (bool*)malloc(sizeof(bool)*N);
    }

    /* initialise */
    mem_set(dp, 0, sizeof(seq_t)*N);
    dp[0] = (seq_t)1;          /* empty configuration, all sums 0 */
    size_t act_cnt = 1;
    act[0] = 0;

    /* early return for n == 0 (offset 1, a(1) is the first term) */
    if (n == 0) return (seq_t)0;

    for (idx_t step = 0; step < n; ++step) {
        mem_set(nxt, 0, sizeof(seq_t)*N);
        memset(seen, 0, sizeof(bool)*N);
        size_t nact_cnt = 0;

        for (size_t ai = 0; ai < act_cnt; ++ai) {
            int s = act[ai];
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;

            uint32_t start = trans_start[s];
            uint32_t finish = trans_start[s+1];
            for (uint32_t ei = start; ei < finish; ++ei) {
                uint16_t ns = trans_next[ei];
                nxt[ns] += val;
                if (!seen[ns]) {
                    seen[ns] = true;
                    nact[nact_cnt++] = ns;
                }
            }
        }
        /* swap vectors */
        seq_t *tmp = dp; dp = nxt; nxt = tmp;
        /* swap active lists */
        memcpy(act, nact, sizeof(int)*nact_cnt);
        act_cnt = nact_cnt;
    }

    /* sum all possibilities */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < N; ++i) result += dp[i];
    return result;
}
