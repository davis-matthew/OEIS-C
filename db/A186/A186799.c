/*
 * A186799  Number of (n+2)X4 binary arrays with no 3X3 subblock commuting
 *          with any horizontal or vertical neighbor 3X3 subblock.
 * https://oeis.org/A186799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   matrix_commutes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A186799 by a transfer‑matrix DP over 3‑row windows. */
/* The sequence counts (n+2)×4 binary arrays with no commuting 3×3 blocks
   horizontally or vertically. The DP keeps all admissible 3‑row states
   and the transitions that respect the vertical non‑commutation. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/matrix_commutes.h"

/* External integer types supplied by the build.  Do NOT modify. */
typedef uint64_t idx_t;   /* index type */

/* ----------------- State representation ----------------- */
typedef struct {
    uint8_t a, b, c;           /* three consecutive rows (4‑bit values)      */
    int    left[9], right[9];  /* 3×3 matrices of the left/right 3×3 blocks */
    uint16_t *neigh;           /* indices of admissible successor states    */
    size_t Nneigh;             /* number of successors                        */
} State;

/* ----------------- Global data (built once) ----------------- */
static bool init_done = false;
static size_t NumStates = 0;
static State *States = NULL;

/* DP caching: results for n = 1 … max_computed */
static seq_t *dp_cur = NULL;   /* DP vector for current step */
static seq_t *dp_next = NULL;  /* DP vector for next step */
static seq_t *cache = NULL;    /* cache[n] = a(n) */
static size_t cache_cap = 0;   /* capacity of cache array (indices 0 … cache_cap-1) */
static size_t max_computed = 0;/* highest n for which cache[n] is valid */

/* ----------------- Helper: horizontal validity of a 3‑row window ----------------- */
static bool horizontal_ok(uint8_t a, uint8_t b, uint8_t c)
{
    uint8_t rows[3] = {a, b, c};
    int left[9], right[9];
    for (int r = 0; r < 3; ++r) {
        uint8_t row = rows[r];
        for (int col = 0; col < 3; ++col) {
            left[r*3 + col]  = (row >> col) & 1;
            right[r*3 + col] = (row >> (col+1)) & 1;
        }
    }
    /* The two 3×3 blocks (columns 0–2 vs 1–3) must not commute */
    return !matrix_commutes(left, right);
}

/* ----------------- Build all states and precedence graph ----------------- */
static void init_states(void)
{
    if (init_done) return;

    /* Map 12‑bit window id to state index (−1 = invalid) */
    int idx_of[1 << 12];
    for (int i = 0; i < (1 << 12); ++i) idx_of[i] = -1;

    int count = 0;
    /* Enumerate all possible 3‑row windows */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 16; ++c)
                if (horizontal_ok((uint8_t)a, (uint8_t)b, (uint8_t)c))
                    idx_of[(a << 8) | (b << 4) | c] = count++;

    NumStates = (size_t)count;
    States = (State *)malloc(NumStates * sizeof(State));

    /* Fill state data */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 16; ++c) {
                int id  = (a << 8) | (b << 4) | c;
                int idx = idx_of[id];
                if (idx < 0) continue;
                State *st = &States[idx];
                st->a = (uint8_t)a;  st->b = (uint8_t)b;  st->c = (uint8_t)c;
                st->Nneigh = 0; st->neigh = NULL;

                /* Precompute left/right submatrices */
                uint8_t rows[3] = {(uint8_t)a, (uint8_t)b, (uint8_t)c};
                for (int r = 0; r < 3; ++r) {
                    uint8_t row = rows[r];
                    for (int col = 0; col < 3; ++col) {
                        st->left [r*3 + col] = (row >> col) & 1;
                        st->right[r*3 + col] = (row >> (col+1)) & 1;
                    }
                }
            }

    /* Build adjacency lists (vertical transitions) */
    for (size_t sidx = 0; sidx < NumStates; ++sidx) {
        State *s = &States[sidx];
        uint16_t buf[16];
        size_t ncnt = 0;
        for (int d = 0; d < 16; ++d) {
            int tid = (s->b << 8) | (s->c << 4) | d;
            int j = idx_of[tid];
            if (j < 0) continue;
            State *t = &States[j];
            if (matrix_commutes(s->left,  t->left))  continue; /* vertical left block */
            if (matrix_commutes(s->right, t->right)) continue; /* vertical right block */
            buf[ncnt++] = (uint16_t)j;
        }
        s->Nneigh = ncnt;
        if (ncnt) {
            s->neigh = (uint16_t *)malloc(ncnt * sizeof(uint16_t));
            memcpy(s->neigh, buf, ncnt * sizeof(uint16_t));
        }
    }

    init_done = true;

    /* Allocate DP arrays and initial cache */
    dp_cur  = (seq_t *)malloc(NumStates * sizeof(seq_t));
    dp_next = (seq_t *)malloc(NumStates * sizeof(seq_t));

    /* a(1) – all admissible 3‑row windows */
    for (size_t i = 0; i < NumStates; ++i) dp_cur[i] = 1;
    seq_t first = 0;
    for (size_t i = 0; i < NumStates; ++i) first += dp_cur[i];

    /* Initialise cache (indices 0 unused) */
    cache_cap = 2;                 /* capacity for indices 0 … 1 */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[1] = first;
    max_computed = 1;
}

/* ----------------- Main sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_states();                     /* ensure global structures built */

    /* Expand cache and DP up to needed n */
    if ((size_t)n > max_computed) {
        /* Grow cache array if necessary */
        if ((size_t)n + 1 > cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 128;
            while (newcap <= (size_t)n + 1) newcap <<= 1;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache_cap = newcap;
        }

        /* DP from max_computed+1 up to n */
        for (idx_t step = max_computed + 1; step <= n; ++step) {
            /* Clear next vector */
            for (size_t i = 0; i < NumStates; ++i) dp_next[i] = 0;

            /* Transition */
            for (size_t i = 0; i < NumStates; ++i) {
                seq_t val = dp_cur[i];
                if (val == (seq_t)0) continue;
                const State *s = &States[i];
                for (size_t k = 0; k < s->Nneigh; ++k) {
                    uint16_t j = s->neigh[k];
                    dp_next[j] += val;
                }
            }

            /* Swap dp vectors */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;

            /* Sum for this n and store */
            seq_t ans = 0;
            for (size_t i = 0; i < NumStates; ++i) ans += dp_cur[i];
            cache[step] = ans;
        }
        max_computed = (size_t)n;
    }

    return cache[n];
}
