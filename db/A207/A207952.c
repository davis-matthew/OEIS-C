/*
 * A207952  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 19, u64 n <= 188, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207952: Number of 6×n 0..1 arrays avoiding
   horizontal 001 and 011, and vertical 010 and 101.
   The algorithm performs a breadth‑first dynamic program over
   admissible column patterns.  Every term is obtained from the
   preceding ones, so the routine is memoised and independent of
   call order.  The recurrence involves only additions and
   small fixed‑size loops, so the implementation runs in  O(n)
   time with a very modest constant.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void init(void);

/* --- global data ----------------------------------------------------- */
static idx_t          series_cap = 0;      /* size of the series array   */
static seq_t          *series = NULL;      /* a(n) for n >= 1            */

static size_t         m = 0;               /* number of admissible columns   */
static uint8_t        *validMask = NULL;   /* masks of admissible columns   */
static uint8_t        *nextPool = NULL;    /* flat list of admissible third columns   */
static size_t        *nextCount = NULL;   /* count of entries in nextPool per pair */
static seq_t          *dp_prev = NULL;    /* DP for current length          */
static seq_t          *dp_cur  = NULL;    /* DP for next length              */
static size_t         computed_len = 0;    /* greatest n that has been processed */
static size_t         total_pairs = 0;     /* = m*m   */

/* --------------------------------------------------------------------- */
static void init(void)
{
    if (series != NULL) return; /* already initialised */

    /* Build list of admissible 6‑bit column masks (no vertical 010 or 101) */
    uint8_t maskStore[64];
    m = 0;
    for (uint32_t mask = 0; mask < 64; ++mask) {
        bool ok = true;
        for (int r = 0; r < 4; ++r) {          /* four triples of rows */
            int tri = ((mask >> r) & 1) << 2 |
                      ((mask >> (r+1)) & 1) << 1 |
                      ((mask >> (r+2)) & 1);
            if (tri == 2 || tri == 5) {        /* 010 or 101 forbidden */
                ok = false;
                break;
            }
        }
        if (ok) maskStore[m++] = (uint8_t)mask;
    }

    /* Copy to a dynamic array */
    validMask = (uint8_t *)malloc(m * sizeof(uint8_t));
    memcpy(validMask, maskStore, m * sizeof(uint8_t));

    /* Pre‑compute adjacency: for each ordered pair (i,j) list of k
       such that (i,j,k) has no horizontal forbidden triple 001 or 011 */
    total_pairs = m * m;
    nextCount = (size_t *)calloc(total_pairs, sizeof(size_t));
    nextPool  = (uint8_t *)malloc(total_pairs * m * sizeof(uint8_t)); /* worst case */

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            size_t pair = i * m + j;
            size_t pos  = pair * m;
            for (size_t k = 0; k < m; ++k) {
                bool ok = true;
                for (int r = 0; r < 6; ++r) {
                    int tri = ((validMask[i] >> r) & 1) << 2 |
                              ((validMask[j] >> r) & 1) << 1 |
                              ((validMask[k] >> r) & 1);
                    if (tri == 1 || tri == 3) {       /* 001 or 011 forbidden */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    nextPool[pos + nextCount[pair]++] = (uint8_t)k;
                }
            }
        }
    }

    /* DP arrays for length 2 (any pair) */
    dp_prev = (seq_t *)calloc(total_pairs, sizeof(seq_t));
    dp_cur  = (seq_t *)calloc(total_pairs, sizeof(seq_t));
    for (size_t p = 0; p < total_pairs; ++p) dp_prev[p] = (seq_t)1;
    computed_len = 2;

    /* Series array, initialise first two terms */
    series_cap = 3;          /* indices 0..2 will be used */
    series = (seq_t *)calloc(series_cap, sizeof(seq_t));
    series[1] = (seq_t)m;
    series[2] = (seq_t)m * (seq_t)m;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();                    /* ensure the globals are ready */

    if (n < 1) return (seq_t)0; /* undefined for n<1 */

    size_t N = (size_t)n;

    /* enlarge series array if needed */
    if (N + 1 > series_cap) {
        size_t new_cap = N + 1;
        series = (seq_t *)realloc(series, new_cap * sizeof(seq_t));
        series_cap = new_cap;
    }

    /* compute new terms sequentially from the last computed length */
    for (size_t len = computed_len + 1; len <= N; ++len) {
        /* dp_prev contains counts for length len‑1 */
        /* compute dp_cur for length len */
        memset(dp_cur, 0, total_pairs * sizeof(seq_t));

        for (size_t pair = 0; pair < total_pairs; ++pair) {
            seq_t v = dp_prev[pair];
            if (v == 0) continue;

            size_t j = pair % m;           /* second column index of the pair */
            size_t start = pair * m;
            size_t cnt   = nextCount[pair];
            for (size_t t = 0; t < cnt; ++t) {
                uint8_t k = nextPool[start + t];
                size_t idx = j * m + (size_t)k;   /* pair (j,k) for next length */
                dp_cur[idx] += v;
            }
        }

        /* swap buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
        computed_len = len;

        /* sum all entries of dp_prev to obtain a(len) */
        seq_t total = 0;
        for (size_t p = 0; p < total_pairs; ++p) {
            total += dp_prev[p];
        }
        series[len] = total;
    }

    return series[N];
}
