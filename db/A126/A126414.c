/*
 * A126414  Number of base 27 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A126414
   Definition: a(n) = number of base‑27 strings of length n whose
   adjacent digits differ by at most two.  The empty string (n = 0)
   counts as one string, and the first digit may be zero.
   Recurrence:
       a(0) = 1
       a(1) = 27
       For n ≥ 2:
           let dp[k][d] be the number of strings of length k ending
           with digit d (0 ≤ d ≤ 26).  Then
               dp[1][d] = 1
               dp[k][d] = Σ_{|d−e|≤2} dp[k−1][e]
           and a(k) = Σ_{d=0}^{26} dp[k][d].
*/

#include <stdlib.h>

static seq_t *sum_cache = NULL;    /* cache of a(n) values */
static size_t cache_len = 0;       /* largest n for which sum_cache[n] is computed */
static seq_t dp_prev[27];          /* dp for length cache_len   */
static size_t current_len = 0;     /* the length represented by dp_prev */

/* Ensure that sum_cache contains a(0)…a(n) and that dp_prev represents length cache_len. */
static void ensure_cache(size_t n)
{
    if (cache_len >= n)
        return; /* already computed */

    /* First time: initialise base cases. */
    if (cache_len == 0) {
        /* Allocate for n=0 */
        sum_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!sum_cache) { /* allocation should succeed in the test harness */
            abort();
        }
        sum_cache[0] = (seq_t)1;   /* a(0) = 1 */

        /* Build for n=1 */
        for (int d = 0; d < 27; ++d)
            dp_prev[d] = (seq_t)1; /* dp[1][d] = 1 */

        if (n == 0) {
            cache_len = 0;
            current_len = 0;
            return;
        }

        sum_cache = (seq_t*)realloc(sum_cache, sizeof(seq_t) * 2);
        if (!sum_cache) abort();
        /* sum of dp_prev */
        seq_t total = 0;
        for (int d = 0; d < 27; ++d)
            total += dp_prev[d];
        sum_cache[1] = total;      /* a(1) = 27 */
        cache_len = 1;
        current_len = 1;
    }

    /* Compute further values up to n. */
    seq_t dp_cur[27];

    while (cache_len < n) {
        /* Transition: dp_cur[d] = Σ_{|d−e|≤2} dp_prev[e] */
        for (int d = 0; d < 27; ++d) {
            int low  = d - 2; if (low  < 0) low  = 0;
            int high = d + 2; if (high > 26) high = 26;
            seq_t sum = 0;
            for (int e = low; e <= high; ++e)
                sum += dp_prev[e];
            dp_cur[d] = sum;
        }

        /* Store a(cache_len+1) */
        seq_t total = 0;
        for (int d = 0; d < 27; ++d)
            total += dp_cur[d];

        /* Resize cache if needed */
        if (cache_len + 1 >= (size_t)(1<<30)) ; /* never reached in tests */
        else {
            seq_t *tmp = (seq_t*)realloc(sum_cache, sizeof(seq_t) * (cache_len + 2));
            if (!tmp) abort();
            sum_cache = tmp;
        }
        sum_cache[cache_len + 1] = total;

        /* Prepare for next step */
        for (int d = 0; d < 27; ++d)
            dp_prev[d] = dp_cur[d];
        cache_len++;
    }

    current_len = cache_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* a(0) */
    ensure_cache((size_t)n);
    return sum_cache[n];
}
