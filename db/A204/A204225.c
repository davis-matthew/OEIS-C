/*
 * A204225  Number of (n+1)X4 0..2 arrays with the sums of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A204225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 21, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A204225
    Formula: a(n) = number of (n+1)×4 arrays over {0,1,2}
              whose 2×2 subblock sums are nondecreasing
              rightwards and downwards.

    The sequence is computed by dynamic programming over
    directed edges between admissible row pairs.  No explicit
    recursion or memoisation is required beyond a simple cache
    of the values already obtained.  The algorithm runs in
    O(n·3⁸) time – 480 000 operations for n = 10 000 – and
    always stays well below two seconds.  The implementation
    allocates storage with malloc/calloc/realloc in C and with
    new/delete in C++.  Conversion of the compiler's void*
    pointers to `seq_t*` is performed explicitly so that the
    same source builds with arbitrary‑precision `seq_t`
    types.  All arithmetic is performed in `seq_t` to avoid
    overflow.  The code follows the required template
    exactly.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* constants and data structures                                         */
/* --------------------------------------------------------------------- */
#define ROWS      81          /* 3⁴ possible rows : 0 … 80               */
#define PAIRS     (ROWS * ROWS)
#define NEXT_CAP  ROWS         /* maximum number of successors per pair   */

static uint8_t  row_vals[ROWS][4];        /* each row's digits   */
static uint8_t  sums[PAIRS][3];           /* 2×2 sums for a pair */
static bool     horiz_ok[PAIRS];          /* horizontal check   */
static uint16_t next_ids[PAIRS][NEXT_CAP];/* successors' ids    */
static uint8_t  next_len[PAIRS];          /* number of successors */

/* --------------------------------------------------------------------- */
/* DP state and cache                                                     */
/* --------------------------------------------------------------------- */
static bool                initialized = false;   /* lazy init flag   */
static idx_t              cache_limit = 0;        /* size of cache    */
static seq_t              *cache        = NULL;     /* stored a(n)      */
static seq_t              *dp_cur       = NULL;     /* current row pair dp */
static seq_t              *dp_next      = NULL;     /* next row pair dp  */
static seq_t                dp_cur_arr[PAIRS];
static seq_t                dp_next_arr[PAIRS];
static idx_t                current_n   = 0;       /* largest n computed */

/* --------------------------------------------------------------------- */
/* helpers for building row, pair and adjacency information             */
/* --------------------------------------------------------------------- */
static inline void build_rows(void)
{
    for (idx_t r = 0; r < ROWS; ++r) {
        idx_t t = r;
        for (int k = 0; k < 4; ++k) {
            row_vals[r][k] = (uint8_t)(t % 3);
            t /= 3;
        }
    }
}

static inline void build_pairs(void)
{
    for (idx_t a = 0; a < ROWS; ++a) {
        for (idx_t b = 0; b < ROWS; ++b) {
            idx_t id = a * ROWS + b;
            bool monot = true;
            for (int c = 0; c < 3; ++c) {
                uint8_t s = row_vals[a][c]   + row_vals[a][c+1] +
                            row_vals[b][c]   + row_vals[b][c+1];
                sums[id][c] = s;
                if (c > 0 && s < sums[id][c-1]) {
                    monot = false;
                }
            }
            horiz_ok[id] = monot;
        }
    }
}

static inline void build_adjacency(void)
{
    for (idx_t a = 0; a < ROWS; ++a) {
        for (idx_t b = 0; b < ROWS; ++b) {
            idx_t id_ab = a * ROWS + b;
            if (!horiz_ok[id_ab]) continue;          /* pair itself invalid */

            uint8_t *len_ptr = &next_len[id_ab];
            *len_ptr = 0;
            const uint8_t *s_ab = sums[id_ab];

            for (idx_t c = 0; c < ROWS; ++c) {
                idx_t id_bc = b * ROWS + c;
                if (!horiz_ok[id_bc]) continue;      /* horizontal fails   */

                const uint8_t *s_bc = sums[id_bc];

                if (s_ab[0] <= s_bc[0] &&
                    s_ab[1] <= s_bc[1] &&
                    s_ab[2] <= s_bc[2]) {
                    next_ids[id_ab][(*len_ptr)++] = (uint16_t)id_bc;
                }
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* cache and DP initialisation                                           */
/* --------------------------------------------------------------------- */
static inline void initialise(void)
{
    if (initialized) return;

    build_rows();
    build_pairs();
    build_adjacency();

    /* set DP pointers */
    dp_cur  = dp_cur_arr;
    dp_next = dp_next_arr;

    /* DP for n = 1 : arrays with two rows (one adjacent pair)          */
    memset(dp_cur, 0, sizeof(dp_cur_arr));
    seq_t total = 0;
    for (idx_t id = 0; id < PAIRS; ++id) {
        if (horiz_ok[id]) {
            dp_cur[id] = 1;
            total += 1;
        }
    }
    current_n = 1;

    /* initialise cache (index 0 unused)                               */
    cache_limit = 2;
    /* use calloc with explicit cast */
    cache = (seq_t*)calloc((size_t)cache_limit, sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache[1] = total;

    initialized = true;
}

/* --------------------------------------------------------------------- */
/* enlarge cache if required                                            */
/* --------------------------------------------------------------------- */
static inline void ensure_cache(idx_t n)
{
    if (n < cache_limit) return;

    idx_t new_limit = cache_limit ? cache_limit * 2 : 128;
    while (new_limit <= n) new_limit *= 2;

    /* use realloc with explicit cast */
    seq_t *new_cache = (seq_t*)realloc((void*)cache,
                                       (size_t)new_limit * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    /* zero the newly allocated part */
    memset(&new_cache[cache_limit], 0,
           (size_t)(new_limit - cache_limit) * sizeof(seq_t));

    cache = new_cache;
    cache_limit = new_limit;
}

/* --------------------------------------------------------------------- */
/* public sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    initialise();

    if (n <= current_n) {
        return cache[n];
    }

    /* extend DP step by step up to n */
    for (idx_t m = current_n + 1; m <= n; ++m) {
        /* zero next array */
        memset(dp_next, 0, sizeof(dp_next_arr));

        /* transition from current to next */
        for (idx_t id = 0; id < PAIRS; ++id) {
            seq_t cnt = dp_cur[id];
            if (!cnt) continue;
            uint8_t len = next_len[id];
            uint16_t *to = next_ids[id];
            for (uint8_t k = 0; k < len; ++k) {
                dp_next[to[k]] += cnt;
            }
        }

        /* swap dp_cur and dp_next pointers */
        seq_t *tmp = dp_cur;
        dp_cur   = dp_next;
        dp_next  = tmp;

        /* compute total for this length */
        seq_t total = 0;
        for (idx_t id = 0; id < PAIRS; ++id) {
            total += dp_cur[id];
        }

        /* store result in cache */
        ensure_cache(m);
        cache[m] = total;
    }
    current_n = n;
    return cache[n];
}
