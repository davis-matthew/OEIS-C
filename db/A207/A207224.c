/*
 * A207224  Number of nX4 0..2 arrays avoiding the patterns z z+1 z or z z-1 z
 *          in any row, column or nw-se diagonal.
 * https://oeis.org/A207224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   bad3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A207224
   Number of n×4 arrays over {0,1,2} avoiding z z+1 z or z z-1 z
   in any row, column or NW–SE diagonal.  The function below
   computes the values with a dynamic‑programming approach that
   stores the sequence up to the largest indexed value requested.
   It is O(n·E) where E≈|states|⋅|transitions| (≈10⁵ operations
   per row), comfortably fast for n≤10000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bad3.h"

/*------------------------------------------------------------------------------*/
/* Utility prototypes – the build provides these */
/* (declarations are only placeholders to keep the compiler happy) */
#ifdef __cplusplus
extern "C" {
#endif
    /* These utilities are not used in this implementation but are
       required by the build system. */
    idx_t _floor_isqrt_u64(idx_t);
    idx_t _prime_limit_for_idx(idx_t);
    uint64_t _smallest_prime_factor(uint64_t);
    bool all_even_digits(seq_t);
#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------*/
/* Constants for the 4‑column rows over {0,1,2} */
enum { MAX_ROW_DIGITS = 4, MODE = 3 };        /* digits 0..2 */
enum { MAX_ROWS = 81 };                       /* 3⁴ */

/*------------------------------------------------------------------------------*/

/* Convert row index (0..80) to 4 digits array. */
static inline void row_to_digits(int idx, int dig[4]) {
    dig[3] = idx % MODE; idx /= MODE;
    dig[2] = idx % MODE; idx /= MODE;
    dig[1] = idx % MODE; idx /= MODE;
    dig[0] = idx;
}

/*------------------------------------------------------------------------------*/
/* Static data initialised on first call. */
static bool _initialised      = false;
static idx_t   _valid_cnt     = 0;          /* number of valid rows    */
static int     _row_of_index[ MAX_ROWS ];/* map 0..80 -> 0.._valid_cnt-1 */
static int     _index_of_row[ MAX_ROWS ];/* map 0.._valid_cnt-1 -> 0..80 */
static int     _valid_rows[ MAX_ROWS ];   /* list of valid row indices */

/* State space: (previous two rows). */
static idx_t   _state_cnt          = 0;     /* valid_cnt * valid_cnt     */
static int*    _adj_start          = NULL; /* start offset of adj list   */
static int*    _adj_count          = NULL; /* number of outgoing edges   */
static int*    _adj_list           = NULL; /* destination state ids      */

/* DP arrays for the current and next row count. */
static seq_t*  _dp_cur   = NULL;
static seq_t*  _dp_next  = NULL;

/* Cache of computed a(n) values (1‑based). */
static seq_t*  _cache    = NULL;
static idx_t   _cache_cap = 0;
static idx_t   _computed_max = 0;

/*------------------------------------------------------------------------------*/
/* Build all valid rows, adjacency lists, and DP infrastructure. */
static void _initialise(void)
{
    /* Step 1: find all rows that avoid the forbidden pattern within a row. */
    int row_digits[MAX_ROWS][MAX_ROW_DIGITS];
    _valid_cnt = 0;
    for (int idx = 0; idx < MAX_ROWS; ++idx) {
        row_to_digits(idx, row_digits[idx]);
        bool ok = true;
        /* check windows [0,1,2] and [1,2,3] */
        for (int pos = 0; pos <= 1 && ok; ++pos) {
            if (bad3(row_digits[idx][pos],
                     row_digits[idx][pos+1],
                     row_digits[idx][pos+2]))
                ok = false;
        }
        if (ok) {
            _valid_rows[_valid_cnt] = idx;
            _row_of_index[idx]       = _valid_cnt;
            _index_of_row[_valid_cnt] = idx;
            ++_valid_cnt;
        }
    }

    _state_cnt = _valid_cnt * _valid_cnt;

    /* Allocate adjacency data.  Maximum edges: state_cnt * _valid_cnt. */
    _adj_start  = (int*)malloc(_state_cnt * sizeof(int));
    _adj_count  = (int*)malloc(_state_cnt * sizeof(int));
    _adj_list   = (int*)malloc(_state_cnt * _valid_cnt * sizeof(int));

    /* Build adjacency: for each pair (r1,r2) find all r3 that fit. */
    int edge_idx = 0;
    for (int s1 = 0; s1 < _valid_cnt; ++s1) {
        int r1 = _index_of_row[s1];
        int d1[4]; row_to_digits(r1, d1);
        for (int s2 = 0; s2 < _valid_cnt; ++s2) {
            int src_id = s1 * _valid_cnt + s2;
            _adj_start[src_id] = edge_idx;
            int out = 0;
            int r2 = _index_of_row[s2];
            int d2[4]; row_to_digits(r2, d2);
            for (int s3 = 0; s3 < _valid_cnt; ++s3) {
                int r3 = _index_of_row[s3];
                int d3[4]; row_to_digits(r3, d3);

                bool ok = true;
                /* vertical columns */
                for (int col = 0; col < 4 && ok; ++col)
                    if (bad3(d1[col], d2[col], d3[col]))
                        ok = false;
                /* NW-SE diagonals */
                if (ok && bad3(d1[0], d2[1], d3[2])) ok = false;
                if (ok && bad3(d1[1], d2[2], d3[3])) ok = false;

                if (ok) {
                    _adj_list[edge_idx++] = s2 * _valid_cnt + s3;
                    ++out;
                }
            }
            _adj_count[src_id] = out;
        }
    }

    /* DP arrays */
    _dp_cur  = (seq_t*)malloc(_state_cnt * sizeof(seq_t));
    _dp_next = (seq_t*)malloc(_state_cnt * sizeof(seq_t));

    /* Cache */
    _cache_cap   = 3;                      /* at least 2 entries (1 and 2)  */
    _cache       = (seq_t*)malloc(_cache_cap * sizeof(seq_t));
    _cache[1] = (seq_t)_valid_cnt;                       /* n = 1   */
    _cache[2] = (seq_t)_valid_cnt * (seq_t)_valid_cnt;   /* n = 2   */
    _computed_max = 2;

    /* initialise dp_cur for n = 2 : each pair has count 1 */
    for (int id = 0; id < _state_cnt; ++id)
        _dp_cur[id] = (seq_t)1;
    _initialised = true;
}

/*------------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, but harmless */

    if (!_initialised) _initialise();

    /* Extend cache up to n if necessary. */
    if (n > _computed_max) {
        /* Ensure cache capacity.  The cache grows by doubling. */
        if (n + 5 > _cache_cap) {
            size_t new_cap = _cache_cap * 2;
            while (new_cap <= n + 5) new_cap *= 2;
            _cache = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
            _cache_cap = new_cap;
        }

        /* For n >= 3 run the DP steps.  _dp_cur holds results for
           current length; _dp_next is temporary.  Initially _dp_cur
           corresponds to length 2 (all pairs). */
        for (idx_t cur_n = _computed_max + 1; cur_n <= n; ++cur_n) {
            /* zero next array */
            memset(_dp_next, 0, _state_cnt * sizeof(seq_t));

            /* transition step */
            for (int src = 0; src < _state_cnt; ++src) {
                seq_t val = _dp_cur[src];
                if (val == (seq_t)0) continue;
                int start = _adj_start[src];
                int count = _adj_count[src];
                for (int k = 0; k < count; ++k) {
                    int dst = _adj_list[start + k];
                    _dp_next[dst] += val;
                }
            }

            /* swap arrays */
            seq_t* tmp = _dp_cur; _dp_cur = _dp_next; _dp_next = tmp;

            /* compute total for this length */
            seq_t sum = (seq_t)0;
            for (int id = 0; id < _state_cnt; ++id)
                sum += _dp_cur[id];

            _cache[cur_n] = sum;
        }

        _computed_max = n;
    }

    return _cache[n];
}
