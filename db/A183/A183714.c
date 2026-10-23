/*
 * A183714  1/20 of the number of (n+1) X 5 0..4 arrays with every 2 X 2
 *          subblock strictly increasing clockwise or counterclockwise with
 *          one decrease.
 * https://oeis.org/A183714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A183714
    Recurrence (empirical):
        a(n) = 6*a(n-1) + 5*a(n-2) - 42*a(n-3) - 2*a(n-4) +
               84*a(n-5) - 20*a(n-6) - 46*a(n-7) + 19*a(n-8) +
               2*a(n-9) - a(n-10)
    Valid for all n ≥ 1 (empirically verified up to many terms).

    For n ≤ 10 the values are computed from the combinatorial definition
    via dynamic programming over 5‑column rows with digits 0…4.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define ROWS 3125      /* 5^5 possible 5‑digit rows */
#define MAXN 10       /* base values computed directly */

/* Storage for digit representation of each row (base‑5). */
static uint8_t row_digits[ROWS][5];

/* Compatibility lists: 'compat[i]' points to an array of rows
   that can follow row i. */
static int *compat[ROWS];
static int  compat_size[ROWS];

/* Sequence cache (1‑based). */
static seq_t *cache = NULL;
static idx_t  cached_last = 0;
static size_t cache_cap = 0;

/* Flag whether initialisation has been performed. */
static int initialized = 0;

/* ------------------------------------------------------------------ */
/* Utility: compute whether two rows are compatible.  This is true
   iff for every column j=0..3 the 2×2 block formed by
   row_i[j], row_i[j+1], row_j[j], row_j[j+1] has 4 distinct digits
   and the cycle of 4 digits has exactly one decrease, in either
   clockwise or counter‑clockwise order.                      */
static int rows_compatible(int i, int j)
{
    uint8_t const *a = row_digits[i];
    uint8_t const *b = row_digits[j];

    for (int col = 0; col < 4; ++col) {
        int tl = a[col];
        int tr = a[col + 1];
        int bl = b[col];
        int br = b[col + 1];

        /* Must all be distinct */
        if (tl == tr || tl == bl || tl == br ||
            tr == bl || tr == br ||
            bl == br) {
            return 0;
        }

        /* Count decreases in clockwise order: tl, tr, br, bl */
        int decCW = 0;
        if (tr < tl) ++decCW;
        if (br < tr) ++decCW;
        if (bl < br) ++decCW;
        if (tl < bl) ++decCW;   /* wrap around */

        if (decCW == 1) continue;

        /* Counter‑clockwise: tl, bl, br, tr */
        int decCCW = 0;
        if (bl < tl) ++decCCW;
        if (br < bl) ++decCCW;
        if (tr < br) ++decCCW;
        if (tl < tr) ++decCCW;   /* wrap around */

        if (decCCW != 1) return 0;
    }
    return 1;
}

/* ------------------------------------------------------------------ */
/* Build digit tables and compatibility lists once. */
static void initialise(void)
{
    if (initialized) return;
    initialized = 1;

    /* Precompute the 5‑digit base‑5 representation of each row. */
    for (int idx = 0; idx < ROWS; ++idx) {
        int n = idx;
        for (int d = 4; d >= 0; --d) {
            row_digits[idx][d] = (uint8_t)(n % 5);
            n /= 5;
        }
    }

    /* First pass: count compatibility lists. */
    for (int i = 0; i < ROWS; ++i) {
        int cnt = 0;
        for (int j = 0; j < ROWS; ++j) {
            if (rows_compatible(i, j)) ++cnt;
        }
        compat_size[i] = cnt;
    }

    /* Allocate lists. */
    for (int i = 0; i < ROWS; ++i) {
        if (compat_size[i]) {
            compat[i] = (int *)malloc(compat_size[i] * sizeof(int));
        } else {
            compat[i] = NULL;
        }
    }

    /* Second pass: fill lists. */
    for (int i = 0; i < ROWS; ++i) {
        int *p = compat[i];
        for (int j = 0; j < ROWS; ++j) {
            if (rows_compatible(i, j)) {
                *p++ = j;
            }
        }
    }

    /* -------------------------------------------------------------- */
    /* Compute first ten terms via dynamic programming.               */

    /* Allocate DP arrays. */
    seq_t *dp_cur  = (seq_t *)calloc(ROWS, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(ROWS, sizeof(seq_t));

    /* Initial row (row 1): any row is allowed. */
    for (int r = 0; r < ROWS; ++r) {
        dp_cur[r] = (seq_t)1;
    }

    /* Cache allocation. */
    cache_cap = 12; /* indices 0..11, 0 unused */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)0;

    /* Compute up to n = 10 (rows = n+1 = 11). */
    for (int step = 1; step <= MAXN; ++step) {
        /* dp_next = 0 */
        memset(dp_next, 0, ROWS * sizeof(seq_t));

        /* Transition: for each prev row, add to all compatible next rows. */
        for (int prev = 0; prev < ROWS; ++prev) {
            seq_t val = dp_cur[prev];
            if (!val) continue;
            int sz = compat_size[prev];
            int *list = compat[prev];
            for (int k = 0; k < sz; ++k) {
                dp_next[list[k]] += val;
            }
        }

        /* total number of arrays of size step+1 rows */
        seq_t total = 0;
        for (int r = 0; r < ROWS; ++r) {
            total += dp_next[r];
        }

        /* a(step) = total / 20 */
        cache[step] = total / (seq_t)20;
        cached_last = step;

        /* Prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Free temporary DP arrays (dp_cur is now the last dp_next). */
    free(dp_next);
    free(dp_cur);
}

/* ------------------------------------------------------------------ */
/* Ensure cache can hold values up to 'need' (inclusive). */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  External linkage, as required. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* out of range */

    initialise();                 /* lazy init on first call */

    /* Expand cache if necessary. */
    if ((size_t)n + 1 > cache_cap)
        ensure_cache_capacity((size_t)n + 1);

    /* Compute further terms if needed using the empirical recurrence. */
    if (n > cached_last) {
        for (idx_t i = cached_last + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;

            /* Positive coefficients */
            pos += (seq_t)6  * cache[i - 1];
            pos += (seq_t)5  * cache[i - 2];
            pos += (seq_t)84 * cache[i - 5];
            pos += (seq_t)19 * cache[i - 8];
            pos += (seq_t)2  * cache[i - 9];

            /* Negative coefficients (subtracted later) */
            neg += (seq_t)42 * cache[i - 3];
            neg += (seq_t)2  * cache[i - 4];
            neg += (seq_t)20 * cache[i - 6];
            neg += (seq_t)46 * cache[i - 7];
            neg += (seq_t)1  * cache[i -10];

            cache[i] = pos - neg;
        }
        cached_last = n;
    }

    return cache[n];
}
