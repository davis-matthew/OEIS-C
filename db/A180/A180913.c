/*
 * A180913  Number of sequences of n integers p(i) i=0..n-1 with 0<=p(i)<=8*i
 *          and -8<p(i)-p(i-1)<=8.
 * https://oeis.org/A180913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1
 *                dp[0][0] = 1
 *                dp[i][u] = Σ_{v=max(0,u‑8)}^{min(8*(i‑1),u+7)} dp[i‑1][v]
 *                a(n)     = Σ_{u=0}^{8*(n‑1)} dp[n‑1][u]
 *
 * This implementation memoises all a(n) computed so far and grows the
 * tables on demand.  It works for any seq_t (fixed‑size or arbitrary‑precision)
 * and any idx_t (e.g. uint64_t).  All allocations use explicit casts so the
 * code compiles in C++17 as well.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* State for memoisation */
static seq_t *a_cache      = NULL;   /* 1‑based array of a(n) values   */
static size_t a_cache_cap  = 0;       /* capacity of a_cache            */
static idx_t  cache_max    = 0;       /* largest n whose a(n) is ready  */

/* State for dynamic‐programming rows */
static seq_t *prev_row     = NULL;   /* dp[row_index] array            */
static idx_t  prev_len     = 0;       /* length of prev_row             */
static idx_t  prev_index   = 0;       /* row index of prev_row          */

/* Ensure a_cache has space for at least index n */
static void ensure_a_cache_cap(idx_t n)
{
    if ((size_t)n < a_cache_cap)
        return;
    size_t newcap = (a_cache_cap > 0) ? a_cache_cap : 1;
    while ((size_t)n >= newcap)
        newcap *= 2;
    a_cache = (seq_t*)realloc(a_cache, newcap * sizeof(seq_t));
    a_cache_cap = newcap;
}

/* Initialise the first row (n = 1) */
static void init_storage(void)
{
    if (cache_max > 0)
        return;

    /* a_cache[1] = 1 */
    a_cache_cap = 2;
    a_cache = (seq_t*)calloc(a_cache_cap, sizeof(seq_t));
    a_cache[1] = (seq_t)1;
    cache_max = 1;

    /* prev_row for i = 0 : only value 0 = 1 */
    prev_len  = 1;
    prev_row  = (seq_t*)calloc(prev_len, sizeof(seq_t));
    prev_row[0] = (seq_t)1;
    prev_index = 0;  /* row index 0 */
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined; safeguard */

    init_storage();                        /* initialise on first call */
    ensure_a_cache_cap(n);

    /* extend the table up to n */
    while (cache_max < n) {
        /* compute next row (row index = cache_max) */
        idx_t curr_index = cache_max;                 /* next i */
        idx_t curr_len   = (idx_t)8 * curr_index + 1; /* 8*i + 1 */

        /* current row array */
        seq_t *curr_row = (seq_t*)calloc((size_t)curr_len, sizeof(seq_t));

        /* prefix sums of previous row for O(1) range sums */
        seq_t *pref = (seq_t*)calloc((size_t)prev_len, sizeof(seq_t));
        pref[0] = prev_row[0];
        for (idx_t j = 1; j < prev_len; ++j)
            pref[j] = pref[j - 1] + prev_row[j];

        seq_t sum_next = (seq_t)0;

        for (idx_t u = 0; u < curr_len; ++u) {
            idx_t lo = (u > (idx_t)8) ? u - (idx_t)8 : (idx_t)0;
            idx_t hi = (u + (idx_t)7 < prev_len) ? u + (idx_t)7 : prev_len - (idx_t)1;

            seq_t s = pref[hi];
            if (lo > (idx_t)0)
                s = s - pref[lo - (idx_t)1];
            curr_row[u] = s;
            sum_next += s;                   /* accumulate a(n) */
        }

        /* store the new term and advance state */
        a_cache[cache_max + (idx_t)1] = sum_next;

        free(prev_row);
        free(pref);
        prev_row   = curr_row;
        prev_len   = curr_len;
        prev_index = curr_index;
        cache_max++;
    }

    return a_cache[n];
}
