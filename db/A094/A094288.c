/*
 * A094288  Number of (s(0), s(1), ..., s(n)) such that 0 < s(i) < 8 and |s(i)
 *          - s(i-1)| <= 1 for i = 1,2,...,n, s(0) = 1, s(n) = 1.
 * https://oeis.org/A094288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(n) = 7*a(n-1) - 15*a(n-2) + 5*a(n-3) + 15*a(n-4)
 *          - 9*a(n-5) - 3*a(n-6) + a(n-7)   for n ≥ 7
 *   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4,
 *                   a(4)=9, a(5)=21, a(6)=51
 *
 * This implementation uses a linear‑time walk DP on the 7 states
 * {1,…,7}.  No negative coefficients are needed and all operations
 * are additions of seq_t which is safe for the arbitrary‑precision
 * C++ builds and for 64‑bit builds up to the first overflow.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static data for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_len = 0;           /* number of values stored (≥ 1) */
    static seq_t *dp[2] = {NULL, NULL};    /* two temporary vectors of size 8 */
    static seq_t *dp_prev = NULL;
    static seq_t *dp_curr = NULL;
    static size_t cur_n = 0;               /* last n for which cache is filled */

    /* Initialise static data on first call */
    if (cache == NULL) {
        cache_capacity = 1;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = 1;        /* a(0) */
        cache_len = 1;

        dp[0] = (seq_t *)malloc(8 * sizeof(seq_t));
        dp[1] = (seq_t *)malloc(8 * sizeof(seq_t));
        dp_prev = dp[0];
        dp_curr = dp[1];
        for (int i = 1; i <= 7; ++i) dp_prev[i] = 0;
        dp_prev[1] = 1;     /* start state at index 0 */
        cur_n = 0;
    }

    /* Grow cache array if needed */
    if ((size_t)n + 1 > cache_capacity) {
        size_t need = (size_t)n + 1;
        size_t new_cap = cache_capacity;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failed; return 0 as fallback */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute values up to n if we haven't yet hit that index */
    if (n >= cache_len) {
        /* Zero the temporary vector for safety before first use */
        for (int i = 1; i <= 7; ++i) dp_curr[i] = 0;

        for (size_t i = cur_n + 1; i <= n; ++i) {
            /* Clear dp_curr for next step */
            for (int j = 1; j <= 7; ++j) dp_curr[j] = 0;

            /* Transition from every state j */
            for (int j = 1; j <= 7; ++j) {
                seq_t val = dp_prev[j];
                dp_curr[j] += val;
                if (j > 1) dp_curr[j] += dp_prev[j - 1];
                if (j < 7) dp_curr[j] += dp_prev[j + 1];
            }

            cache[i] = dp_curr[1];        /* a(i) = number of walks ending at state 1 */

            /* Swap vectors for next iteration */
            seq_t *tmp = dp_prev;
            dp_prev = dp_curr;
            dp_curr = tmp;
            cur_n = i;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
