/*
 * A207577  Number of nX3 0..3 arrays avoiding the pattern z+1 z+1 z
 *          horizontally and z-1 z-1 z vertically.
 * https://oeis.org/A207577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2,3} avoiding the patterns
 *            horizontal (z+1,z+1,z) for z=0..2 and vertical (z-1,z-1,z) for
 *            z=1..3. The algorithm counts admissible rows, then uses dynamic
 *            programming over successive rows
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */

/* ------------------------------------------------------------------ */
/* Internal state and tables                                            */
/* ------------------------------------------------------------------ */
static bool          _inited    = false;
static idx_t         _max_n     = 0;
static seq_t       *_ans       = NULL;   /* answer cache, indices 0..n */
static size_t        _ans_sz    = 0;
static seq_t       *_dp_cur    = NULL;
static seq_t       *_dp_next   = NULL;

/* counts of admissible rows and states */
static int           _R = 0;            /* number of admissible rows (61) */
static int           _S = 0;            /* number of states = _R*_R (3721) */

/* adjacency (flat array): dests[state*61 + k] is the k‑th successor of state;
   outdeg[state] is the number of successors. */
static int        *_adj_dests = NULL;
static int        *_outdeg    = NULL;

/* digits of each admissible row */
static int         _row_digits[61][3];
static int         _row_enc[61];

/* ------------------------------------------------------------------ */
/* Build tables: admissible rows and adjacency                         */
/* ------------------------------------------------------------------ */
static void _build_tables(void)
{
    /* enumerate admissible rows: all 3‑digit base‑4 strings except
       110, 221, 332 (these contain a forbidden horizontal pattern). */
    int rcnt = 0;
    for (int code = 0; code < 64; ++code) {
        int d0 = (code & 3);
        int d1 = ((code >> 2) & 3);
        int d2 = ((code >> 4) & 3);
        if (d0 == 1 && d1 == 1 && d2 == 0) continue; /* 110 */
        if (d0 == 2 && d1 == 2 && d2 == 1) continue; /* 221 */
        if (d0 == 3 && d1 == 3 && d2 == 2) continue; /* 332 */
        _row_enc[rcnt] = code;
        _row_digits[rcnt][0] = d0;
        _row_digits[rcnt][1] = d1;
        _row_digits[rcnt][2] = d2;
        ++rcnt;
    }
    _R = rcnt;                 /* 61 */
    _S = _R * _R;              /* 3721 */

    /* adjacency lists */
    _adj_dests = (int*)malloc(_S * _R * sizeof(int));
    _outdeg    = (int*)malloc(_S * sizeof(int));
    if (!_adj_dests || !_outdeg) abort();

    for (int prev2 = 0; prev2 < _R; ++prev2) {
        for (int prev1 = 0; prev1 < _R; ++prev1) {
            int state = prev2 * _R + prev1;
            int base  = state * _R;
            int deg   = 0;
            for (int cand = 0; cand < _R; ++cand) {
                bool ok = true;
                for (int col = 0; col < 3; ++col) {
                    int a = _row_digits[prev2][col];
                    int b = _row_digits[prev1][col];
                    int c = _row_digits[cand][col];
                    if (a == b && a + 1 == c) { ok = false; break; }
                }
                if (ok) {
                    _adj_dests[base + deg] = prev1 * _R + cand;
                    ++deg;
                }
            }
            _outdeg[state] = deg;
        }
    }

    /* DP vectors for successive rows */
    _dp_cur   = (seq_t*)malloc(_S * sizeof(seq_t));
    _dp_next  = (seq_t*)malloc(_S * sizeof(seq_t));
    if (!_dp_cur || !_dp_next) abort();

    /* initialize for n = 2: every pair of admissible rows occurs once */
    for (idx_t i = 0; i < (idx_t)_S; ++i) _dp_cur[i] = (seq_t)1;

    /* answer cache: at least up to n=2 */
    _ans_sz = 3;                        /* indices 0..2 */
    _ans = (seq_t*)calloc(_ans_sz, sizeof(seq_t));
    if (!_ans) abort();

    _ans[1] = (seq_t)_R;                /* a(1) = 61 */
    {
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < (idx_t)_S; ++i) sum += _dp_cur[i];
        _ans[2] = sum;                  /* a(2) = 3721 */
    }
    _max_n = 2;
}

/* ------------------------------------------------------------------ */
/* Helper utilities                                                     */
/* ------------------------------------------------------------------ */
static inline void _ensure_inited(void)
{
    if (!_inited) {
        _build_tables();
        _inited = true;
    }
}

static inline void _grow_ans(idx_t n)
{
    if (n + 1 <= _ans_sz) return;
    size_t new_sz = (n + 1) * 2;
    seq_t *tmp = (seq_t*)realloc(_ans, new_sz * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = _ans_sz; i < new_sz; ++i) tmp[i] = (seq_t)0;
    _ans = tmp;
    _ans_sz = new_sz;
}

static inline void _advance(idx_t n)
{
    while (_max_n < n) {
        /* zero next vector */
        for (size_t i = 0; i < (size_t)_S; ++i) _dp_next[i] = (seq_t)0;

        /* transition */
        for (int state = 0; state < _S; ++state) {
            seq_t cur = _dp_cur[state];
            if (!cur) continue;
            int deg = _outdeg[state];
            int base = state * _R;
            for (int k = 0; k < deg; ++k) {
                int dest = _adj_dests[base + k];
                _dp_next[dest] += cur;
            }
        }

        /* swap vectors */
        seq_t *tmp = _dp_cur;
        _dp_cur = _dp_next;
        _dp_next = tmp;

        /* total for current n */
        seq_t tot = (seq_t)0;
        for (size_t i = 0; i < (size_t)_S; ++i) tot += _dp_cur[i];

        _grow_ans(_max_n + 1);
        _ans[_max_n + 1] = tot;
        ++_max_n;
    }
}

/* ------------------------------------------------------------------ */
/* Public function a(n)                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    _ensure_inited();
    if (n == 0) return (seq_t)0;
    if (n <= _max_n) return _ans[n];
    _grow_ans(n);
    _advance(n);
    return _ans[n];
}
