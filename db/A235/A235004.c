/*
 * A235004  Number of (n+1) X (3+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Uses a transfer‑matrix dynamic‑programming (DP) approach. The DP
 *            enumerates all 4‑column patterns for a row (2744 states),
 *            computes transitions that respect the 2×2 diagonal‑sum rule and
 *            the "no equal orthogonal neighbours" rule, and then iteratively
 *            propagates the number of ways to extend arrays...
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_seq_t_array, reallocate_seq_t_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/alloc_seq_t_array.h"
#include "../utils/reallocate_seq_t_array.h"

/* ------------------------------------------------------------------ */
/*  Helper routines for allocating seq_t arrays that work for both   */
/*  C and C++ (seq_t may be an arbitrary‑precision type in C++).       */

static void free_seq_t_array(seq_t *arr) {
#if defined(__cplusplus)
    delete [] arr;
#else
    free(arr);
#endif
}

/* ------------------------------------------------------------------ */
/*  Global state                                                */
/* ------------------------------------------------------------------ */
typedef struct {
    size_t N;              /* number of row patterns (2744)                */
    uint8_t (*states)[4];  /* array of row patterns                       */
    int **succ;            /* adjacency: succ[i] list of successor indices */
    int *succ_cnt;         /* number of successors for each state          */
    seq_t *cur;            /* DP vector for current height                */
    seq_t *next;           /* DP vector for next height                   */
    seq_t *cache;          /* memoised a(n) values (index starts 1)        */
    size_t cache_len;      /* highest n computed                           */
    size_t cache_cap;      /* allocated size of cache                      */
} a235004_state_t;

static a235004_state_t _state;
static bool _initialized = false;

/* ------------------------------------------------------------------ */
/*  Build all valid row patterns and their successor lists          */
/* ------------------------------------------------------------------ */
static void _build_states_and_transitions(void)
{
    /* Enumerate all 4‑tuples with no equal adjacent entries */
    const size_t MAX_STATES = 2744;                  /* 8*7*7*7 */
    uint8_t (*states)[4] = (uint8_t (*)[4])malloc(MAX_STATES * sizeof(uint8_t[4]));
    if (!states) abort();

    int index_map[8][8][8][8];
    memset(index_map, -1, sizeof(index_map));

    size_t N = 0;
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b) {
            if (b == a) continue;
            for (int c = 0; c < 8; ++c) {
                if (c == b) continue;
                for (int d = 0; d < 8; ++d) {
                    if (d == c) continue;
                    states[N][0] = (uint8_t)a;
                    states[N][1] = (uint8_t)b;
                    states[N][2] = (uint8_t)c;
                    states[N][3] = (uint8_t)d;
                    index_map[a][b][c][d] = (int)N;
                    N++;
                }
            }
        }
    if (N != MAX_STATES) abort();  /* invariant violation */

    /* Allocate successor data structures */
    int **succ = (int**)malloc(N * sizeof(int*));
    int *succ_cnt = (int*)calloc(N, sizeof(int));
    if (!succ || !succ_cnt) abort();

    for (size_t i = 0; i < N; ++i) {
        int p[4] = {states[i][0], states[i][1], states[i][2], states[i][3]};
        /* Try all possible first column values for the next row */
        for (int c0 = 0; c0 < 8; ++c0) {
            if (c0 == p[0]) continue;
            /* For each choice of ±2 signs for the 3 column pairs */
            for (int mask = 0; mask < 8; ++mask) {
                int c[4];
                c[0] = c0;
                bool ok = true;
                for (int j = 0; j < 3 && ok; ++j) {
                    int diff = p[j+1] - p[j];
                    int delta = diff + (((mask >> j) & 1) ? 2 : -2);
                    int next_val = c[j] + delta;
                    if (next_val < 0 || next_val > 7) { ok = false; break; }
                    if (next_val == p[j+1]) { ok = false; break; }      /* vertical */
                    if (next_val == c[j])   { ok = false; break; }      /* horizontal */
                    c[j+1] = next_val;
                }
                if (!ok) continue;
                int idx = index_map[c[0]][c[1]][c[2]][c[3]];
                if (idx < 0) continue;
                int cur_cnt = succ_cnt[i];
                if (!succ[i]) {
                    succ[i] = (int*)malloc(4 * sizeof(int));
                } else {
                    succ[i] = (int*)realloc(succ[i], (cur_cnt + 1) * sizeof(int));
                }
                succ[i][cur_cnt] = idx;
                succ_cnt[i] = cur_cnt + 1;
            }
        }
    }

    /* Store in global state */
    _state.N = N;
    _state.states = states;
    _state.succ = succ;
    _state.succ_cnt = succ_cnt;

    /* Allocate DP vectors */
    _state.cur  = alloc_seq_t_array(N);
    _state.next = alloc_seq_t_array(N);
    for (size_t i = 0; i < N; ++i) _state.cur[i] = (seq_t)1;   /* one row */

    /* Allocate minimal cache for memoisation */
    _state.cache_cap = 2;
    _state.cache = alloc_seq_t_array(_state.cache_cap);
    /* cache[0] unused; cache_len counts highest index computed */
    _state.cache_len = 0;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for offset 0, return 0 */

    if (!_initialized) {
        _build_states_and_transitions();
        _initialized = true;
    }

    /* Compute terms up to requested n */
    while (_state.cache_len < (size_t)n) {
        /* Transition step: cur -> next */
        size_t N = _state.N;
        for (size_t i = 0; i < N; ++i) _state.next[i] = (seq_t)0;

        for (size_t i = 0; i < N; ++i) {
            seq_t val = _state.cur[i];
            int *list = _state.succ[i];
            int cnt   = _state.succ_cnt[i];
            for (int k = 0; k < cnt; ++k) {
                _state.next[list[k]] += val;
            }
        }

        /* Sum all entries of next to obtain a(k+1) */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < N; ++i) total += _state.next[i];

        /* Expand cache if needed */
        size_t next_idx = _state.cache_len + 1;
        if (next_idx >= _state.cache_cap) {
            size_t new_cap = _state.cache_cap ? _state.cache_cap * 2 : 2;
            while (new_cap <= next_idx) new_cap *= 2;
            _state.cache = reallocate_seq_t_array(_state.cache,
                                                   _state.cache_cap,
                                                   new_cap);
            _state.cache_cap = new_cap;
        }
        _state.cache[next_idx] = total;
        _state.cache_len = next_idx;

        /* Prepare for next iteration */
        seq_t *tmp = _state.cur;
        _state.cur = _state.next;
        _state.next = tmp;
    }

    return _state.cache[(size_t)n];
}
