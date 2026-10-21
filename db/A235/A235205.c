/*
 * A235205  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Seq. A235205: number of (n+1)×2 0..7 arrays
 *  with every 2×2 subblock having its diagonal sum differing
 *  from its antidiagonal sum by 5 and with no two equal
 *  horizontally or vertically adjacent elements.
 *
 *  The value is computed by dynamic programming on the 56
 *  possible ordered pairs (x,y), x≠y, of row elements.
 *  For each step we extend the row count using all allowed
 *  transitions that satisfy the constraints.
 *
 *  The first few values are: 240,1172,5696,27812,135520,662892,...
 *
 *  This implementation memoizes computed terms up to the largest
 *  `n` requested, so repeated calls are inexpensive.
 */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */
#include <stdlib.h>   /* for malloc, realloc */

/* Function prototype (to be provided by the build system) */
static void seq_free(seq_t *p);

/* Internal data */
static seq_t *answer_cache = NULL;    /* cache of a(n) values (1‑indexed)      */
static size_t cache_cap     = 0;      /* allocated size of answer_cache    */
static size_t cache_used    = 0;      /* number of terms currently stored  */

static seq_t dp_cur[56];              /* DP for current number of rows      */
static seq_t dp_next[56];             /* DP for the next row               */
static int   trans[56][56];           /* adjacency lists (up to 56 entries) */
static int   deg[56];                 /* number of outgoing transitions    */
static bool  initialized = false;     /* one‑time initialization flag      */

/* ------------------------------------------------------------------------- */
/* Build the transition graph independent of n.                               */
/* ------------------------------------------------------------------------- */
static void build_transitions(void)
{
    int xs[56], ys[56];   /* coordinates of the 56 states   */
    int idx = 0;
    /* enumerate all ordered pairs (x,y) with x≠y, 0≤x,y<8 */
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (x == y) continue;
            xs[idx] = x;
            ys[idx] = y;
            ++idx;
        }
    }

    /* compute transition lists */
    for (int i = 0; i < 56; ++i) {
        deg[i] = 0;
        for (int j = 0; j < 56; ++j) {
            /* vertical adjacency: rows must differ elementwise */
            if (xs[i] == xs[j]) continue;
            if (ys[i] == ys[j]) continue;

            /* diagonal difference condition |(x + y') - (y + x')| == 5 */
            int diff = (xs[i] + ys[j]) - (ys[i] + xs[j]);
            if (diff < 0) diff = -diff;
            if (diff != 5) continue;

            trans[i][deg[i]++] = j;
        }
    }
}

/* ------------------------------------------------------------------------- */
/* Extend the cache of a(n) values up to index `n` (inclusive).  The DP      */
/* arrays are kept for the current number of rows (current_n).                */
/* ------------------------------------------------------------------------- */
static void extend_cache(idx_t n)
{
    /* Initialize on first call */
    if (!initialized) {
        build_transitions();
        /* DP for one row: all 56 states are possible as start */
        for (int i = 0; i < 56; ++i) dp_cur[i] = (seq_t)1;
        /* no terms stored yet (answer_cache[0] unused) */
        cache_cap   = 16;
        answer_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_used  = 0;
        initialized = true;
    }

    /* current_n is the number of rows represented by dp_cur.
     * Initially dp_cur corresponds to 1 row (n = 0). */
    static size_t current_n = 0;

    while (current_n < n) {
        /* compute dp_next for one additional row */
        for (int i = 0; i < 56; ++i) dp_next[i] = (seq_t)0;
        for (int i = 0; i < 56; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0) continue;
            for (int t = 0; t < deg[i]; ++t) {
                int j = trans[i][t];
                dp_next[j] += val;
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < 56; ++i) dp_cur[i] = dp_next[i];

        /* compute sum of all ending state counts → a(current_n + 1) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 56; ++i) sum += dp_cur[i];

        ++current_n;   /* now we have current_n rows, answer for n = current_n */

        /* store the value */
        if (current_n >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap <= current_n) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(answer_cache, new_cap * sizeof(seq_t));
            answer_cache = tmp;
            cache_cap = new_cap;
        }
        answer_cache[current_n] = sum;
        cache_used = current_n + 1;   /* not strictly needed */
    }
}

/* ------------------------------------------------------------------------- */
/* Public function required by the problem statement.                         */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    extend_cache(n);
    return answer_cache[n];
}
