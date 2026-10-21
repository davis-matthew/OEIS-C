/*
 * A091891  Number of partitions of n into parts which are a sum of exactly as
 *          many distinct powers of 2 as n has 1's in its binary
 *          representation.
 * https://oeis.org/A091891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into parts whose popcount
 *            equals popcount(n)
 * Range:     u8 n <= 47, u16 n <= 128, u32 n <= 512, u64 n <= 1536,
 *            u128 n <= 5631, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external types */
extern seq_t a(idx_t n);

/* static helpers and tables ------------------------------------------------- */
static idx_t popcount_u64(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

/* Global storage for DP tables of all possible popcount values */
static seq_t **dp = NULL;      /* dp[w][i] – number of partitions of i with parts of popcount w */
static size_t dp_w_count = 0;  /* number of popcount slots (w = 0 .. dp_w_count-1) */
static idx_t computed_max_n = 0;

/* Recompute DP tables up to new_max_n (allocating from scratch) */
static void recompute_up_to(idx_t new_max_n)
{
    /* free old tables if any */
    if (dp != NULL) {
        for (size_t w = 0; w < dp_w_count; ++w) {
            free(dp[w]);
        }
        free(dp);
    }

    /* determine number of bits needed for new_max_n, hence maximum popcount */
    size_t bits = 0;
    idx_t tmp = new_max_n;
    while (tmp) { bits++; tmp >>= 1; }
    if (bits == 0) bits = 1;            /* handle new_max_n == 1 case */
    dp_w_count = bits + 1;              /* w ranges 0 .. bits */

    dp = (seq_t **)malloc(dp_w_count * sizeof(seq_t *));
    if (!dp) abort();

    /* allocate and init DP arrays for each popcount w */
    for (size_t w = 0; w < dp_w_count; ++w) {
        dp[w] = (seq_t *)calloc(new_max_n + 1, sizeof(seq_t));
        if (!dp[w]) abort();
        dp[w][0] = 1;                  /* empty partition */
    }

    /* main DP computation: coin‑change style with unlimited parts */
    for (idx_t p = 1; p <= new_max_n; ++p) {
        idx_t w = popcount_u64(p);
        seq_t *arr = dp[w];
        for (idx_t i = p; i <= new_max_n; ++i) {
            arr[i] += arr[i - p];     /* unlimited use of part p */
        }
    }

    computed_max_n = new_max_n;
}

/* Main sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* a(0) = 1 by definition */

    if (n > computed_max_n) recompute_up_to(n);

    idx_t w = popcount_u64(n);
    return dp[w][n];
}
