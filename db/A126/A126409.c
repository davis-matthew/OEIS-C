/*
 * A126409  Number of base 22 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126409: Number of base-22 n-digit numbers with adjacent digits
 * differing by at most two.
 *
 * Let dp_k[d] be the number of length-k strings that end with digit d
 * (0 ≤ d < 22).  Then
 *
 *   dp_1[d] = 1  (for all d)
 *
 *   dp_{k+1}[d] = Σ_{|d'−d|≤2} dp_k[d']   (sum over all digits d' within
 *                                    two steps of d)
 *
 * The answer is
 *
 *   a(n) = 1            for n = 0
 *   a(n) = Σ_{d=0}^{21} dp_n[d]   for n ≥ 1
 *
 * The implementation memoises all computed values in a linked‑in array:
 * subsequent calls recompute only the missing higher values.
 */

#include <stdlib.h>

/* Constant number of digits in base 22. */
#define BASE_DIGITS 22

static seq_t *a_cache = NULL;          /* a_cache[i] holds a(i) */
static idx_t a_cache_size = 0;         /* allocated size of a_cache (in entries) */
static idx_t a_computed_max = 0;       /* highest n that has been computed */

static seq_t *dp_prev = NULL;          /* dp array for current length */
static seq_t *dp_curr = NULL;          /* dp array for next length */

seq_t a(idx_t n) {
    /* Ensure cache large enough for index n. */
    if (n >= a_cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure, caller must handle */
        a_cache = new_cache;
        a_cache_size = new_size;
    }

    /* Handle the trivial case n == 0. */
    if (n == 0) {
        /* initialise a(0) on first call */
        if (a_computed_max == 0 && !a_cache[0]) {
            a_cache[0] = 1;
            a_computed_max = 0;
        }
        return a_cache[0];
    }

    /* Initialise DP arrays and base case a(1) on first use. */
    if (a_computed_max == 0) {
        /* Allocate DP arrays. */
        dp_prev = (seq_t *)realloc(dp_prev, BASE_DIGITS * sizeof(seq_t));
        dp_curr = (seq_t *)realloc(dp_curr, BASE_DIGITS * sizeof(seq_t));
        if (!dp_prev || !dp_curr) return 0; /* allocation failure */

        /* dp_1[d] = 1 for all d. */
        for (int d = 0; d < BASE_DIGITS; ++d) {
            dp_prev[d] = 1;
        }
        a_cache[0] = 1;
        a_cache[1] = BASE_DIGITS;  /* sum of dp_1 */
        a_computed_max = 1;
    }

    /* Compute up to required n if necessary. */
    for (idx_t l = a_computed_max + 1; l <= n; ++l) {
        /* Compute dp_curr for length l. */
        for (int d = 0; d < BASE_DIGITS; ++d) {
            seq_t sum = 0;
            /* add dp_prev[d] */
            sum += dp_prev[d];
            if (d > 0)  sum += dp_prev[d - 1];
            if (d > 1)  sum += dp_prev[d - 2];
            if (d < BASE_DIGITS - 1) sum += dp_prev[d + 1];
            if (d < BASE_DIGITS - 2) sum += dp_prev[d + 2];
            dp_curr[d] = sum;
        }

        /* Sum all dp_curr entries to get a(l). */
        seq_t total = 0;
        for (int d = 0; d < BASE_DIGITS; ++d) {
            total += dp_curr[d];
        }
        a_cache[l] = total;

        /* Prepare for next iteration: dp_prev <- dp_curr. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    a_computed_max = n;
    return a_cache[n];
}
