/*
 * A234494  Number of (n+1) X (4+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays over {0..5} in which each 2×2 *
 *            subblock has diagonal sum differing from its antidiagonal sum by
 *            2. * The implementation uses a transfer matrix on the four
 *            difference * values d = (a0‑a1, a1‑a2, a2‑a3, a3‑a4) ∈ [‑5,5]⁴.
 *            A state is * represented by such a vector tha...
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool _init_done = false;
static idx_t _states_cnt = 0;          /* number of non‑zero weight states */

/* per‑state data */
static seq_t * _w = NULL;              /* weight (number of rows) of each state */
static int (* _diffs)[4] = NULL;       /* difference vector of each state */
static int * _state_from_diff = NULL;  /* mapping diff index → state index, -1 if none */

/* adjacency list */
static size_t * _offsets = NULL;       /* offset in _edges for each state */
static int * _edges = NULL;            /* adjacency list (state indices) */

/* DP vectors */
static seq_t * _dp_cur = NULL;
static seq_t * _dp_next = NULL;

/* cached values of a(n) */
static seq_t * _a_seq = NULL;          /* 1‑based, index 0 unused */
static size_t _a_seq_cap = 0;
static idx_t _max_n_cached = 0;

#define MAX_DIFF 11   /* indices 0..10 correspond to diffs -5..5 */

static inline idx_t diff_vec_index(const int d0, const int d1,
                                   const int d2, const int d3)
{
    return (((((d0 + 5) * MAX_DIFF + (d1 + 5)) * MAX_DIFF
              + (d2 + 5)) * MAX_DIFF) + (d3 + 5));
}

static void _init_once(void)
{
    if (_init_done) return;
    _init_done = true;

    /* ---------- count how many rows produce each difference vector ---------- */
    const idx_t DIFF_SPACE = MAX_DIFF * MAX_DIFF * MAX_DIFF * MAX_DIFF; /* 14641 */
    int *diff_counts = (int *)calloc(DIFF_SPACE, sizeof(int));
    if (!diff_counts) { exit(1); }

    for (int a0 = 0; a0 <= 5; ++a0)
        for (int a1 = 0; a1 <= 5; ++a1)
            for (int a2 = 0; a2 <= 5; ++a2)
                for (int a3 = 0; a3 <= 5; ++a3)
                    for (int a4 = 0; a4 <= 5; ++a4)
    {
        int d0 = a0 - a1;
        int d1 = a1 - a2;
        int d2 = a2 - a3;
        int d3 = a3 - a4;
        idx_t idx = diff_vec_index(d0, d1, d2, d3);
        diff_counts[idx]++;
    }

    /* ---------- identify states that actually occur ---------- */
    for (idx_t i = 0; i < DIFF_SPACE; ++i)
        if (diff_counts[i] != 0) _states_cnt++;

    _w = (seq_t *)malloc(_states_cnt * sizeof(seq_t));
    _diffs = (int (*)[4])malloc(_states_cnt * sizeof(int[4]));
    _state_from_diff = (int *)malloc(DIFF_SPACE * sizeof(int));
    if (!_w || !_diffs || !_state_from_diff) exit(1);
    memset(_state_from_diff, -1, DIFF_SPACE * sizeof(int));

    size_t s = 0;
    for (idx_t idx = 0; idx < DIFF_SPACE; ++idx)
    {
        if (diff_counts[idx] == 0) continue;
        /* decode diff vector from index */
        int tmp = (int)idx;
        int d3 = tmp % MAX_DIFF - 5; tmp /= MAX_DIFF;
        int d2 = tmp % MAX_DIFF - 5; tmp /= MAX_DIFF;
        int d1 = tmp % MAX_DIFF - 5; tmp /= MAX_DIFF;
        int d0 = tmp - 5;
        _diffs[s][0] = d0;
        _diffs[s][1] = d1;
        _diffs[s][2] = d2;
        _diffs[s][3] = d3;
        _w[s] = (seq_t)diff_counts[idx];
        _state_from_diff[idx] = (int)s;
        s++;
    }
    free(diff_counts);

    /* ---------- build adjacency lists ---------- */
    size_t *deg = (size_t *)calloc(_states_cnt, sizeof(size_t));
    if (!deg) exit(1);

    for (size_t i = 0; i < _states_cnt; ++i)
    {
        const int *d = _diffs[i];
        for (int mask = 0; mask < 16; ++mask)   /* 4 bits: sign choices */
        {
            int e[4];
            bool ok = true;
            for (int j = 0; j < 4; ++j)
            {
                int step = (mask & (1 << j)) ? 2 : -2;
                e[j] = d[j] + step;
                if (e[j] < -5 || e[j] > 5) { ok = false; break; }
            }
            if (!ok) continue;
            idx_t e_idx = diff_vec_index(e[0], e[1], e[2], e[3]);
            if (_state_from_diff[e_idx] != -1) deg[i]++;
        }
    }

    _offsets = (size_t *)malloc((_states_cnt + 1) * sizeof(size_t));
    if (!_offsets) exit(1);
    _offsets[0] = 0;
    for (size_t i = 0; i < _states_cnt; ++i)
        _offsets[i + 1] = _offsets[i] + deg[i];

    size_t total_edges = _offsets[_states_cnt];
    _edges = (int *)malloc(total_edges * sizeof(int));
    if (!_edges) exit(1);

    /* fill edges */
    size_t *ptr = (size_t *)malloc(_states_cnt * sizeof(size_t));
    if (!ptr) exit(1);
    for (size_t i = 0; i < _states_cnt; ++i) ptr[i] = _offsets[i];

    for (size_t i = 0; i < _states_cnt; ++i)
    {
        const int *d = _diffs[i];
        for (int mask = 0; mask < 16; ++mask)
        {
            int e[4];
            bool ok = true;
            for (int j = 0; j < 4; ++j)
            {
                int step = (mask & (1 << j)) ? 2 : -2;
                e[j] = d[j] + step;
                if (e[j] < -5 || e[j] > 5) { ok = false; break; }
            }
            if (!ok) continue;
            idx_t e_idx = diff_vec_index(e[0], e[1], e[2], e[3]);
            int neigh = _state_from_diff[e_idx];
            if (neigh != -1)
                _edges[ptr[i]++] = neigh;
        }
    }
    free(ptr);
    free(deg);

    /* ---------- initialize DP vectors ---------- */
    _dp_cur = (seq_t *)calloc(_states_cnt, sizeof(seq_t));
    _dp_next = (seq_t *)calloc(_states_cnt, sizeof(seq_t));
    if (!_dp_cur || !_dp_next) exit(1);
    for (size_t i = 0; i < _states_cnt; ++i)
        _dp_cur[i] = _w[i];

    /* ---------- prepare cache for a(n) ---------- */
    _a_seq_cap = 16;
    _a_seq = (seq_t *)calloc(_a_seq_cap, sizeof(seq_t));
    if (!_a_seq) exit(1);
    /* index 0 unused, a_seq[1] will be filled first time */
    _max_n_cached = 0;
}

static void _ensure_a_cache(idx_t n)
{
    if ((size_t)n < _a_seq_cap) return;
    size_t new_cap = _a_seq_cap;
    while ((size_t)n >= new_cap) new_cap <<= 1;
    _a_seq = (seq_t *)realloc(_a_seq, new_cap * sizeof(seq_t));
    if (!_a_seq) exit(1);
    for (size_t i = _a_seq_cap; i < new_cap; ++i) _a_seq[i] = (seq_t)0;
    _a_seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* below offset, undefined */
    _init_once();

    if (n > _max_n_cached)
    {
        _ensure_a_cache(n);
        for (idx_t step = _max_n_cached + 1; step <= n; ++step)
        {
            /* compute dp_next from dp_cur */
            memset(_dp_next, 0, _states_cnt * sizeof(seq_t));

            for (size_t i = 0; i < _states_cnt; ++i)
            {
                seq_t val = _dp_cur[i];
                if (val == (seq_t)0) continue;
                size_t start = _offsets[i];
                size_t end   = _offsets[i + 1];
                for (size_t k = start; k < end; ++k)
                {
                    int e = _edges[k];
                    _dp_next[e] += val * _w[e];
                }
            }

            /* sum all dp_next entries to get a(step) */
            seq_t sum = (seq_t)0;
            for (size_t i = 0; i < _states_cnt; ++i)
                sum += _dp_next[i];
            _a_seq[step] = sum;

            /* swap dp_cur and dp_next */
            seq_t *tmp = _dp_cur;
            _dp_cur = _dp_next;
            _dp_next = tmp;
        }
        _max_n_cached = n;
    }
    return _a_seq[n];
}
