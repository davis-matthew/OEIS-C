/*
 * A227862  A boustrophedon triangle.
 * https://oeis.org/A227862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) is the nth entry of the “boustrophedon triangle”. T(0,0)=1.
 *            For r>0: T(r,0)=1 and T(r,k)=T(r,k-1)+T(r-1,r-k) (1 ≤ k ≤ r).
 *            The sequence a(n) lists the rows of the triangle, reading the
 *            rows left‑to‑right when the row index r is odd and right‑to‑left
 *            when r is even
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 124, u64 n <= 299,
 *            u128 n <= 783, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* storage of all triangle values   */
static size_t cache_size = 0;        /* number of elements stored       */
static size_t cache_capacity = 0;    /* allocated capacity               */
static idx_t  max_row = 0;           /* last computed row (0‑based)     */
static int    initialized = 0;       /* flag for one‑time initialization */

/* one‑time initialization: create row 0 */
static void init_once(void)
{
    if (initialized) return;
    initialized = 1;

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_capacity = 1;

    cache[0] = (seq_t)1;   /* row 0 : [1] */
    cache_size = 1;
    max_row = 0;
}

/* enlarge the cache so that all rows up to index r are present   */
static void ensure_rows(idx_t r)
{
    init_once();
    while (max_row < r) {
        /* new row to create */
        idx_t new_r    = max_row + 1;        /* row index */
        idx_t prev_len = new_r;              /* length of previous row r‑1 */
        idx_t new_len  = new_r + 1;          /* length of new row r       */

        /* grow the byte buffer if needed */
        if (cache_capacity < cache_size + new_len) {
            size_t new_cap = cache_size + new_len;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* compute the new row into a temporary buffer */
        static seq_t temp[4096];
        temp[0] = (seq_t)1;
        /* start index of the previous row (row r‑1) */
        idx_t prev_start = (new_r - 1) * new_r / 2;
        for (idx_t k = 1; k < new_len; ++k) {
            idx_t idx_prev = prev_start + (prev_len - k);
            temp[k] = temp[k - 1] + cache[idx_prev];
        }

        /* copy the new row into the main cache */
        size_t dest_start = cache_size;
        for (idx_t k = 0; k < new_len; ++k)
            cache[dest_start + k] = temp[k];
        cache_size += new_len;
        max_row = new_r;
    }
}

/* binary search for the row r that contains the index n
   (so that (r+1)(r+2)/2 > n >= r(r+1)/2)             */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* exponential search to find an upper bound */
    while (1) {
        unsigned __int128 h  = high;
        unsigned __int128 cum = (h + 1) * (h + 2) / 2;
        if (cum > n) break;
        if (high > ((idx_t)1 << 62)) break;   /* safe guard */
        high <<= 1;
    }

    /* binary search inside [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 m   = mid;
        unsigned __int128 cum = (m + 1) * (m + 2) / 2;
        if (cum > n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r      = find_row(n);                     /* row index   */
    idx_t start  = r * (r + 1) / 2;                 /* first pos of row r in the linear array */
    idx_t offset = n - start;                       /* position in the row (0‑based)   */
    idx_t col    = (r & 1) ? offset : (r - offset);/* column in left‑to‑right order   */

    ensure_rows(r);                                 /* make sure row r exists */
    return cache[start + col];
}
