/*
 * A126489  Number of base 21 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑21 n‑digit numbers whose adjacent digits
 *            differ by ≤3. Computed by a simple DP: dp[d] = number of valid
 *            n‑digit numbers ending with digit d
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is cached so that calls with arbitrary order of n remain fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *a_cache   = NULL;     /* cache of a(n) values, allocated dynamically   */
static size_t cache_cap = 0;        /* current capacity of a_cache                  */
static idx_t  computed_n = 0;       /* largest n for which a_cache[n] is set        */
static seq_t  dp_prev[21];          /* distribution for current length              */
static seq_t  dp_curr[21];          /* temporary distribution for next length       */
static bool   initialized = false;  /* true after the first call                   */

seq_t a(idx_t n)
{
    /* ---------- one‑time initialisation --------------------------------- */
    if (!initialized) {
        /* allocate enough space for indices 0 and 1 */
        cache_cap = 4;
        a_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!a_cache) abort();

        a_cache[0] = (seq_t)1;                 /* a(0) */
        for (int d = 0; d < 21; ++d)            /* dp for length 1 */
            dp_prev[d] = (seq_t)1;
        a_cache[1] = (seq_t)21;                /* a(1) */
        computed_n = 1;
        initialized = true;
    }

    /* ---------- return if already computed ----------------------------- */
    if (n <= computed_n)
        return a_cache[n];

    /* ---------- extend cache up to the requested n --------------------- */
    while (computed_n < n) {
        /* compute dp_curr for next length */
        for (int d = 0; d < 21; ++d) {
            seq_t sum = 0;
            int start = d - 3;
            if (start < 0) start = 0;
            int end = d + 3;
            if (end > 20) end = 20;
            for (int k = start; k <= end; ++k)
                sum += dp_prev[k];
            dp_curr[d] = sum;
        }

        /* total number of length‑(computed_n+1) numbers */
        seq_t total = 0;
        for (int d = 0; d < 21; ++d)
            total += dp_curr[d];

        /* ensure cache has space for the new value */
        idx_t next_len = computed_n + 1;
        if (next_len >= cache_cap) {
            size_t new_cap = cache_cap;
            while (next_len >= new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
            cache_cap = new_cap;
        }
        a_cache[next_len] = total;

        /* prepare for next iteration */
        for (int d = 0; d < 21; ++d)
            dp_prev[d] = dp_curr[d];
        computed_n = next_len;
    }

    return a_cache[n];
}
