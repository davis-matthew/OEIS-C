/*
 * A046044  Sum of 13 but no fewer nonzero fourth powers.
 * https://oeis.org/A046044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046044: Sum of 13 but no fewer nonzero fourth powers.
   a(n) is the n-th smallest integer whose minimum number of
   fourth‑power summands is exactly 13.  The value is found by
   dynamic programming on the coin set {k⁴ | k ≥ 1}.                               */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Precompiled data structures ------------------------------------------------ */

static int *dp_int          = NULL;   /* minimal coin count for each value   */
static size_t dp_cap        = 0;       /* allocated size for dp_int           */

static idx_t *coins         = NULL;   /* list of available fourth powers     */
static size_t coin_cnt      = 0;       /* number of coins currently stored    */
static size_t coin_cap      = 0;       /* allocated size for coins            */
static idx_t coin_next_k    = 1;       /* next k to generate coin k⁴          */

static seq_t *terms         = NULL;   /* list of produced terms (a(·))       */
static size_t terms_len     = 0;       /* how many terms are stored           */
static size_t terms_cap     = 0;       /* allocated size for terms array      */

static idx_t current_limit  = 0;       /* largest value already computed in dp */

/* --------------------------------------------------------------------------- */

/* Append a value to the global terms array, expanding it as needed.              */
static void _append_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Generate and store all fourth powers up to the given limit.                   */
static void _ensure_coins_up_to(idx_t limit)
{
    while (1) {
        idx_t k = coin_next_k;
        idx_t kk = k * k;                 /* k² */
        /* overflow in 64‑bit?  it fits for the limited limits we support.      */
        idx_t pk = kk * kk;                /* k⁴ */
        if (pk > limit) break;
        if (coin_cnt >= coin_cap) {
            size_t new_cap = coin_cap ? coin_cap * 2 : 16;
            coins = (idx_t *)realloc(coins, new_cap * sizeof(idx_t));
            coin_cap = new_cap;
        }
        coins[coin_cnt++] = pk;
        coin_next_k++;
    }
}

/* Extend the dynamic programming table up to at least `n` terms.                 */
static void _ensure_terms(idx_t n)
{
    /* If we already have enough terms, do nothing.                               */
    if (terms_len >= n)
        return;

    /* Increase the DP limit progressively until we gather enough terms.         */
    idx_t old_limit = current_limit;
    idx_t new_limit = old_limit;
    while (terms_len < n) {
        /* Double the limit or grow by a fixed amount if the limit is zero.      */
        new_limit = new_limit ? new_limit * 2 : 131072;   /* 128 k initial goal   */
        if (new_limit <= current_limit)
            new_limit = current_limit + 131072;          /* ensure growth         */

        /* Resize the dp array if necessary.                                    */
        if (new_limit + 1 > dp_cap) {
            size_t new_cap = dp_cap ? dp_cap * 2 : (new_limit + 1);
            if (new_cap < new_limit + 1)
                new_cap = new_limit + 1;
            dp_int = (int *)realloc(dp_int, new_cap * sizeof(int));
            dp_cap = new_cap;
        }

        /* Mark the newly added entries as "infinite" except for index 0.        */
        for (idx_t i = old_limit + 1; i <= new_limit; ++i)
            dp_int[i] = INT_MAX / 2;    /* safe large value for min */

        /* Ensure the list of coins covers the new limit.                      */
        _ensure_coins_up_to(new_limit);

        /* Compute minimal counts for the new range [old_limit+1, new_limit].  */
        for (idx_t i = old_limit + 1; i <= new_limit; ++i) {
            int best = INT_MAX / 2;
            for (size_t c = 0; c < coin_cnt; ++c) {
                idx_t coin = coins[c];
                if (coin > i)
                    break;          /* coins are sorted increasingly */
                int cand = dp_int[i - coin] + 1;
                if (cand < best)
                    best = cand;
            }
            dp_int[i] = best;
            if (best == 13)
                _append_term((seq_t)i);
        }

        old_limit = new_limit;
        current_limit = new_limit;
    }
}

/* --------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* First request initializes all static data structures.                     */
    if (!dp_int)
        dp_int = (int *)calloc(1, sizeof(int));   /* dp[0] = 0 initially       */
    if (!dp_int[0])                                  /* guard against uninit.     */
        dp_int[0] = 0;

    _ensure_terms(n);
    return terms[n - 1];
}
