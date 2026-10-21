/*
 * A233869  Number of (n+1)X(1+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 21.
 * https://oeis.org/A233869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays of digits 0..7 with every 2×2
 *            block having the sum of the absolute values of the six edge and
 *            diagonal differences equal to 21. Computation is performed with
 *            dynamic programming: (1) 64 possible row states (two digits),
 *            0..63. (2) Precompute for every pair of rows...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t and seq_t are provided by the build; do not #define or
   include them explicitly.  All arithmetic uses seq_t operations
   (addition, subtraction, etc.). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>

static bool initialized = false;

static seq_t dp[64];
static seq_t dp_next[64];
static int  next_list[64][64];
static int  next_cnt[64];

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_max = 0;

/* Helper to compute the adjacency lists.  Called once. */
static void build_transitions(void)
{
    for (int p = 0; p < 64; ++p) {
        int a0 = p / 8;
        int b0 = p % 8;
        next_cnt[p] = 0;
        for (int q = 0; q < 64; ++q) {
            int a1 = q / 8;
            int b1 = q % 8;

            /* sum of absolute differences of the six edges and diagonals */
            int sum = abs(a0 - b0)   /* horizontal top */
                    + abs(a1 - b1)   /* horizontal bottom */
                    + abs(a0 - a1)   /* vertical left */
                    + abs(b0 - b1)   /* vertical right */
                    + abs(a0 - b1)   /* diagonal down-right */
                    + abs(b0 - a1);  /* diagonal down-left */

            if (sum == 21) {
                next_list[p][next_cnt[p]++] = q;
            }
        }
    }

    /* initial dp after 0 transitions : any first row allowed */
    for (int i = 0; i < 64; ++i) {
        dp[i] = 1;                 /* literal 1 converts to seq_t */
    }
}

/* Ensure that the cache array can hold values up to n. */
static void grow_cache(size_t n)
{
    if (n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;

    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort.  In a real system you may handle
           this differently, but for the purposes of this exercise
           we simply exit. */
        exit(1);
    }
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main exported function. */
seq_t a(idx_t n)
{
    /* Ensure the transition data is ready. */
    if (!initialized) {
        build_transitions();
        /* Allocate cache large enough for the initial index range. */
        cache = NULL;
        cache_cap = 0;
        grow_cache(1000);          /* start with a size that covers many calls */
        cache_max = 0;
        cache[0] = 0;              /* unused */
        initialized = true;
    }

    if (n == 0) return 0;          /* sequence is defined from n ≥ 1 */

    /* Grow cache if necessary. */
    grow_cache((size_t)n);

    /* Compute sequence values up to n if not already done. */
    while (cache_max < (size_t)n) {
        /* 0‑copy dp_next */
        for (int i = 0; i < 64; ++i) dp_next[i] = 0;

        /* Transition step: dp_next[q] = Σ_{p→q} dp[p] */
        for (int p = 0; p < 64; ++p) {
            seq_t val = dp[p];
            if (val == 0) continue;
            for (int k = 0; k < next_cnt[p]; ++k) {
                int q = next_list[p][k];
                dp_next[q] += val;
            }
        }

        /* Prepare for next iteration */
        for (int i = 0; i < 64; ++i) dp[i] = dp_next[i];
        cache_max++;

        /* Sum all dp entries to get a(cache_max) */
        seq_t sum = 0;
        for (int i = 0; i < 64; ++i) sum += dp[i];
        cache[cache_max] = sum;
    }

    return cache[(size_t)n];
}
