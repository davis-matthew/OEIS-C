/*
 * A129263  Skylar (age 7) counts change by stacking all coins of the same
 *          type then arranging the stacks in a row. a(n) is the number of
 *          distinct Skylar stackings of n cents using any combination of
 *          pennies, nickels, dimes or quarters.
 * https://oeis.org/A129263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A129263 – Skylar stackings
    a(n) = number of distinct stackings of n cents
           using pennies (1), nickels (5), dimes (10) and quarters (25).
    For each non‑empty subset S of the four coin types,
    let D(S) = sum of the denominations in S
    and let m = |S|.
    The number of ways to choose positive counts for the coin types in S
    that sum to n is equal to the number of solutions of
        sum_{i∈S} x_i * d_i = n – D(S)   (x_i ≥ 0),
    i.e. the partition function with coins in S.
    Hence
        a(n) = Σ_{S≠∅}  m! * ways_S(n – D(S))   and a(0) = 1.  */
/* The array dp[s][k] holds the number of ways to make amount k
   with the coin types in subset s (s ∈ [1..15]).
   All arithmetic is performed in type seq_t. */

#include <stdlib.h>   /* malloc, free, size_t */

static const idx_t DENOM[4] = {1, 5, 10, 25};              /* coin values          */
static const seq_t FACT[5] = {(seq_t)1, (seq_t)1, (seq_t)2,
                              (seq_t)6, (seq_t)24};        /* 0! .. 4!             */

static idx_t SUBSET_DSUM[16];                            /* D(S) for each subset     */
static idx_t SUBSET_PLC[16];                             /* |S| count for each subset*/
static int SUBSETS_INITIALIZED = 0;                      /* lazy init flag           */

static seq_t *DP = NULL;                                 /* DP table                */
static idx_t DP_MAX_N = 0;                                /* largest n already built  */

/* -------------------------------------------------------------------- */
/*  Initialize the constant tables for the 16 subsets of coin types.       */
static void init_subsets(void) {
    for (idx_t s = 0; s < 16; ++s) {
        idx_t sum = 0;
        idx_t pc  = 0;
        for (int i = 0; i < 4; ++i) {
            if (s & ((idx_t)1 << i)) {
                sum += DENOM[i];
                ++pc;
            }
        }
        SUBSET_DSUM[s] = sum;
        SUBSET_PLC[s]  = pc;
    }
    SUBSETS_INITIALIZED = 1;
}

/* -------------------------------------------------------------------- */
/*  Build the DP table up to new_max_n (inclusive).  The table has shape
    16 x (new_max_n+1) with 0th row unused.                              */
static void build_dp(idx_t new_max_n) {
    /* Free previous table if it exists */
    if (DP) free(DP);

    size_t stride = (size_t)(new_max_n + 1);
    size_t total  = stride * 16;
    DP = (seq_t*)malloc(total * sizeof(seq_t));
    if (!DP) exit(1);   /* memory failure */

    /* Initialise all entries to 0 */
    for (size_t i = 0; i < total; ++i) DP[i] = (seq_t)0;

    /* Set the base for each subset: one way to make amount 0  */
    for (idx_t s = 1; s < 16; ++s) {
        DP[(size_t)s * stride + 0] = (seq_t)1;
    }

    /* Perform the standard coin‑change DP for each non-empty subset */
    for (idx_t s = 1; s < 16; ++s) {
        size_t base = (size_t)s * stride;
        for (int i = 0; i < 4; ++i) {
            if (s & ((idx_t)1 << i)) {
                idx_t coin = DENOM[i];
                for (idx_t amt = coin; amt <= new_max_n; ++amt) {
                    DP[base + amt] += DP[base + amt - coin];
                }
            }
        }
    }

    DP_MAX_N = new_max_n;
}

/* -------------------------------------------------------------------- */
/*  Main function required by the specification.                         */
seq_t a(idx_t n) {
    if (!SUBSETS_INITIALIZED) init_subsets();

    /* Handle the special case n = 0: the empty change is one way.      */
    if (n == 0) return (seq_t)1;

    /* Rebuild DP table if we now need a larger n */
    if (n > DP_MAX_N) build_dp(n);

    idx_t stride = (idx_t)(DP_MAX_N + 1);
    seq_t result = (seq_t)0;

    /* Sum the contributions of all non‑empty subsets */
    for (idx_t s = 1; s < 16; ++s) {
        idx_t dsum = SUBSET_DSUM[s];
        if (n < dsum) continue;          /* impossible to use this subset */

        idx_t remaining = n - dsum;
        seq_t ways = DP[(size_t)s * stride + remaining];
        if (ways == 0) continue;

        idx_t m = SUBSET_PLC[s];
        result += ways * FACT[m];
    }

    return result;
}
