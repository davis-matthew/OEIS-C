/*
 * A212398  Number of binary arrays of length n+7 with no more than 4 ones in
 *          any length 8 subsequence (=50% duty cycle).
 * https://oeis.org/A212398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 29, u64 n <= 67, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = number of binary arrays of length n+7
   with at most 4 ones in any contiguous subsequence of length 8.
   Uses dynamic programming over 7‑bit suffixes. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset = 1, but return 0 for n=0 */

    /* --- cache for values a(1..current_max) --- */
    static seq_t *cache = NULL;          /* pointer to array of seq_t */
    static size_t cache_cap = 0;          /* allocated size (including dummy 0) */

    /* --- DP arrays for 7‑bit suffix counts --- */
    static seq_t dp_cur[128];
    static seq_t dp_next[128];
    static int dp_initialized = 0;       /* flag whether dp_cur is initialized */
    static idx_t current_len = 7;       /* length that has been processed so far */

    /* --- popcount table for 7‑bit numbers --- */
    static unsigned char popcnt[128];
    static int popcnt_built = 0;
    if (!popcnt_built) {
        for (int i = 0; i < 128; ++i) {
            int c = 0, x = i;
            while (x) { c += x & 1; x >>= 1; }
            popcnt[i] = (unsigned char)c;
        }
        popcnt_built = 1;
    }

    /* Ensure cache can hold up to index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;   /* include dummy 0 index */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – unlikely */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Initialize DP for length 7 (no window constraint yet) */
    if (!dp_initialized) {
        for (int s = 0; s < 128; ++s) dp_cur[s] = 1;  /* each 7‑bit pattern occurs once */
        dp_initialized = 1;
    }

    /* Extend DP up to required length n+7 */
    while (current_len < n + 7) {
        /* clear next array */
        memset(dp_next, 0, sizeof(dp_next));

        /* transition */
        for (int s = 0; s < 128; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;          /* skip zero counts */
            int pc = popcnt[s];          /* ones in previous 7 bits */

            /* append 0 */
            if (pc <= 4) {
                int t = (s << 1) & 0x7F; /* shift left, keep 7 bits */
                dp_next[t] += val;
            }

            /* append 1 */
            if (pc + 1 <= 4) {
                int t = ((s << 1) & 0x7F) | 1;
                dp_next[t] += val;
            }
        }

        /* sum all counts for the new length -> a(current_len+1-7) */
        seq_t sum = 0;
        for (int t = 0; t < 128; ++t) sum += dp_next[t];
        cache[(size_t)(current_len + 1 - 7)] = sum;     /* store a(n) */

        /* prepare for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
        current_len++;
    }

    return cache[(size_t)n];
}
