/*
 * A207776  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n 0..1 arrays avoiding horizontally 000 and
 *            111 and vertically 001 and 010
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXV 64
#define MAXS (MAXV*MAXV)

static bool _vertical_ok[1<<6];
static uint8_t  _valid_col[MAXV];
static int      _valid_cnt;
static int      _adj[MAXS][MAXV];
static int      _adj_cnt[MAXS];
static int      _V;
static int      _S;
static seq_t*   _cache = NULL;
static size_t   _cache_cap = 0;
static size_t   _cache_len = 0;
static seq_t*   _cur   = NULL;
static seq_t*   _next  = NULL;
static size_t   _cur_len = 0;

static bool is_valid_col(uint8_t m)
{
    for (int p = 0; p <= 3; ++p) {
        int a = (m >> p) & 1;
        int b = (m >> (p+1)) & 1;
        int c = (m >> (p+2)) & 1;
        if (a==0 && b==0 && c==1) return false;
        if (a==0 && b==1 && c==0) return false;
    }
    return true;
}

static bool triple_ok(uint8_t a, uint8_t b, uint8_t c)
{
    for (int r = 0; r < 6; ++r) {
        int bi =  (a >> r) & 1;
        int bj =  (b >> r) & 1;
        int bk =  (c >> r) & 1;
        if (bi==bj && bj==bk) return false;
    }
    return true;
}

static void _init_once(void)
{
    if (_cur) return; /* already initialized */

    /* precompute vertical validity */
    for (int m = 0; m < (1<<6); ++m) _vertical_ok[m] = is_valid_col(m);

    /* collect valid columns */
    _valid_cnt = 0;
    for (int m = 0; m < (1<<6); ++m)
        if (_vertical_ok[m]) _valid_col[_valid_cnt++] = (uint8_t)m;

    _V = _valid_cnt;
    _S = _V * _V;

    /* adjacency: for each pair (i,j) store dest indices (j,k) where triple_ok */
    for (int i = 0; i < _V; ++i) {
        for (int j = 0; j < _V; ++j) {
            int idx = i * _V + j;
            int cnt = 0;
            uint8_t ci = _valid_col[i];
            uint8_t cj = _valid_col[j];
            for (int k = 0; k < _V; ++k) {
                uint8_t ck = _valid_col[k];
                if (triple_ok(ci, cj, ck)) {
                    int dest = j * _V + k;
                    _adj[idx][cnt++] = dest;
                }
            }
            _adj_cnt[idx] = cnt;
        }
    }

    /* allocate DP arrays */
    _cur  = (seq_t*)malloc(_S * sizeof(seq_t));
    _next = (seq_t*)malloc(_S * sizeof(seq_t));
    memset(_cur, 0, _S * sizeof(seq_t));
    memset(_next, 0, _S * sizeof(seq_t));

    /* initial count for length 2: all pairs valid => 1 each */
    for (int idx = 0; idx < _S; ++idx) _cur[idx] = (seq_t)1;
    _cur_len = 2;

    /* allocate cache for sequence terms */
    _cache_cap = 64;
    _cache = (seq_t*)malloc(_cache_cap * sizeof(seq_t));
    memset(_cache, 0, _cache_cap * sizeof(seq_t));

    /* store a(1) and a(2) */
    _cache[1] = (seq_t)_V;       /* number of valid columns */
    _cache[2] = (seq_t)_S;       /* number of pairs */
    _cache_len = 2;
}

static void _ensure_cache_capacity(size_t need)
{
    if (need < _cache_cap) return;
    size_t new_cap = _cache_cap * 2;
    if (new_cap <= need) new_cap = need + 1;
    seq_t* new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    memcpy(new_cache, _cache, _cache_len * sizeof(seq_t));
    free(_cache);
    _cache = new_cache;
    _cache_cap = new_cap;
}

static void _extend_cache_to(size_t n)
{
    for (size_t step = _cache_len + 1; step <= n; ++step) {
        /* zero _next */
        memset(_next, 0, _S * sizeof(seq_t));

        /* transition */
        for (int idx = 0; idx < _S; ++idx) {
            seq_t cnt = _cur[idx];
            int out_cnt = _adj_cnt[idx];
            for (int t = 0; t < out_cnt; ++t) {
                int dest = _adj[idx][t];
                _next[dest] += cnt;
            }
        }

        /* sum to get a(step) */
        seq_t sum = 0;
        for (int idx = 0; idx < _S; ++idx) sum += _next[idx];

        _ensure_cache_capacity(step);
        _cache[step] = sum;
        _cache_len = step;

        /* swap cur and next */
        seq_t* tmp = _cur;
        _cur = _next;
        _next = tmp;
        _cur_len = step;
    }
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N == 0) return (seq_t)0;  /* Off‑by‑one handling */

    _init_once();

    if (N > _cache_len) _extend_cache_to(N);
    return _cache[N];
}
