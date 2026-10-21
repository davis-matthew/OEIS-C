/*
 * A204627  Number of (n+1)X6 0..2 arrays with every 2X2 subblock having
 *          unequal diagonal elements or unequal antidiagonal elements, and
 *          new values 0..2 introduced in row major order.
 * https://oeis.org/A204627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of (n+1)×6 arrays over {0,1,2} such that every 2×2
 *            subblock has unequal diagonal or unequal antidiagonal elements,
 *            and new values appear in row‑major order
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   max_from_idx, max_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence (empirical linear recurrence) of order 15 holds
   after the first 15 terms. A(1) … A(15) are obtained by a
   transfer‑matrix computation that respects the “new values in
   row‑major order” restriction. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/max_from_idx.h"
#include "../utils/max_to_idx.h"

#define ROWS 729                /* 3^6 possible rows */
#define MAX_COLS 6
#define MAX_STATES (ROWS * 4)   /* 4 possible maxima per row: -1,0,1,2 */

typedef struct { unsigned char d[MAX_COLS]; } Row;
typedef struct { uint16_t nextRow; uint8_t nextMax; } Transition;

/* Pre‑computed data */
static Row all_rows[ROWS];
static Transition **trans;    /* adjacency + new‑value order */
static size_t *trans_cnt;
static size_t *trans_cap;
static uint16_t *first_states;
static size_t first_cap = 0, first_cnt = 0;

/* Cache of computed values */
static seq_t *cache = NULL;      /* cache[1..max] */
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
static bool initialized = false;

/* Build all 729 rows */
static void build_rows(void) {
    for (size_t i = 0; i < ROWS; ++i) {
        int x = (int)i;
        for (int pos = MAX_COLS-1; pos >= 0; --pos) {
            all_rows[i].d[pos] = (unsigned char)(x % 3);
            x /= 3;
        }
    }
}

/* Push a transition from state s to target state t */
static void push_transition(size_t s, uint16_t nextRow, uint8_t nextMax) {
    if (trans[s] == NULL) {
        trans[s] = (Transition*)malloc(64 * sizeof(Transition));
        trans_cap[s] = 64;
        trans_cnt[s] = 0;
    }
    if (trans_cnt[s] == trans_cap[s]) {
        trans_cap[s] <<= 1;
        trans[s] = (Transition*)realloc(trans[s], trans_cap[s] * sizeof(Transition));
    }
    trans[s][trans_cnt[s]++] = (Transition){ nextRow, nextMax };
}

/* Build transition table for every (prev_row, current_max) state */
static void build_transitions(void) {
    trans = (Transition**)calloc(MAX_STATES, sizeof(Transition*));
    trans_cnt = (size_t*)calloc(MAX_STATES, sizeof(size_t));
    trans_cap = (size_t*)calloc(MAX_STATES, sizeof(size_t));

    for (size_t prev = 0; prev < ROWS; ++prev) {
        for (size_t mIdx = 0; mIdx < 4; ++mIdx) {
            int currMax = max_from_idx(mIdx);
            size_t stateIdx = prev * 4 + mIdx;

            for (size_t nxt = 0; nxt < ROWS; ++nxt) {
                /* adjacency constraint */
                bool ok = true;
                for (int k = 0; k < MAX_COLS-1; ++k) {
                    if (all_rows[prev].d[k] == all_rows[nxt].d[k+1] &&
                        all_rows[prev].d[k+1] == all_rows[nxt].d[k]) {
                        ok = false; break;
                    }
                }
                if (!ok) continue;

                /* new‑value order constraint */
                int m = currMax;
                bool valid = true;
                for (int pos = 0; pos < MAX_COLS; ++pos) {
                    int d = all_rows[nxt].d[pos];
                    if (d > m && d != m + 1) { valid = false; break; }
                    if (d == m + 1) ++m;
                }
                if (!valid) continue;

                size_t nextMaxIdx = max_to_idx(m);
                push_transition(stateIdx, (uint16_t)nxt, (uint8_t)nextMaxIdx);
            }
        }
    }
}

/* Build initial states: rows reachable from start max -1 */
static void build_initial_states(void) {
    first_cap = 64;
    first_states = (uint16_t*)malloc(first_cap * sizeof(uint16_t));

    for (size_t row = 0; row < ROWS; ++row) {
        int m = -1;
        bool valid = true;
        for (int pos = 0; pos < MAX_COLS; ++pos) {
            int d = all_rows[row].d[pos];
            if (d > m && d != m + 1) { valid = false; break; }
            if (d == m + 1) ++m;
        }
        if (!valid) continue;
        size_t maxIdx = max_to_idx(m);
        if (first_cnt == first_cap) {
            first_cap <<= 1;
            first_states = (uint16_t*)realloc(first_states, first_cap * sizeof(uint16_t));
        }
        first_states[first_cnt++] = (uint16_t)(row * 4 + maxIdx);
    }
}

/* Compute first 15 terms (a(1) … a(15)) */
static void compute_initial_terms(seq_t *terms, size_t count) {
    /* DP arrays */
    seq_t *cur  = (seq_t*)calloc(MAX_STATES, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(MAX_STATES, sizeof(seq_t));

    /* initial row(s) : after 1 row */
    for (size_t s = 0; s < first_cnt; ++s)
        cur[first_states[s]] = 1;

    /* iterate for next rows */
    for (size_t step = 1; step <= count; ++step) {
        memset(next, 0, MAX_STATES * sizeof(seq_t));
        for (size_t s = 0; s < MAX_STATES; ++s) {
            if (cur[s] == 0) continue;
            size_t tcnt = trans_cnt[s];
            for (size_t t = 0; t < tcnt; ++t) {
                size_t tgt = (size_t)trans[s][t].nextRow * 4 + trans[s][t].nextMax;
                next[tgt] += cur[s];
            }
        }
        /* sum over all states -> a(step) */
        seq_t sum = 0;
        for (size_t i = 0; i < MAX_STATES; ++i) sum += next[i];
        terms[step-1] = sum;  /* terms[0] = a(1) */

        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;
    }
    free(cur); free(next);
}

/* Initialize cache and tables on first call */
static void init_cache(void) {
    if (initialized) return;
    initialized = true;

    build_rows();
    build_transitions();
    build_initial_states();

    /* Allocate cache for at least 15 terms plus small room */
    cache_cap = 32;
    cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));

    seq_t init_terms[15];
    compute_initial_terms(init_terms, 15);
    for (idx_t i = 1; i <= 15; ++i) cache[i] = init_terms[i-1];
    cache_len = 15;
}

/* Recurrence coefficients (order 15) */
static const long long coeff[15] = {
    460,          /* a(n-1)   */
   -23188,        /* a(n-2)   */
   435694,        /* a(n-3)   */
  -3658549,       /* a(n-4)   */
 11358936,        /* a(n-5)   */
 20419834,        /* a(n-6)   */
-191355445,      /* a(n-7)   */
159553869,        /* a(n-8)   */
865499344,        /* a(n-9)   */
-1132300188,      /* a(n-10)  */
-1371731191,      /* a(n-11)  */
1522080017,       /* a(n-12)  */
5670605,          /* a(n-13)  */
-262860202,       /* a(n-14)  */
47382216           /* a(n-15)  */
};

/* Main function required by the problem statement */
seq_t a(idx_t n) {
    init_cache();

    if (n <= cache_len) return cache[n];

    /* Grow cache array if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill intermediate terms using recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t pos_sum = 0, neg_sum = 0;
        for (int i = 0; i < 15; ++i) {
            seq_t term = cache[k - i - 1];
            long long c = coeff[i];
            if (c >= 0)
                pos_sum += term * (seq_t)c;
            else
                neg_sum += term * (seq_t)(-c);
        }
        cache[k] = pos_sum - neg_sum;  /* guaranteed non‑negative */
    }

    cache_len = n;
    return cache[n];
}
