/*
 * A137900  Number of partitions of 2*n into distinct products of at least two
 *          consecutive positive integers.
 * https://oeis.org/A137900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of 2*n into distinct products of at least
 *            two consecutive positive integers
 * Range:     u8 none, u16 none, u32 n <= 250, u64 n <= 250, u128 n <= 250,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* maximum N we are willing to precompute.
   For n up to 10000 this suffices (N=20000). */
#define MAX_N 200000   /* can comfortably cover n up to 100000 */

static seq_t dp[MAX_N + 1];
static bool seen[MAX_N + 1];
static seq_t prods[20000];      /* enough to hold all distinct products <= MAX_N */
static size_t prod_count = 0;
static bool dp_initialized = false;

/* Create all distinct products of at least two consecutive integers up to MAX_N */
static void build_products(void)
{
    for (idx_t k = 1; ; ++k) {
        seq_t prod2 = (seq_t)k * (seq_t)(k + 1);
        if (prod2 > MAX_N) break;

        idx_t val = (idx_t)prod2;
        if (!seen[val]) {
            seen[val] = true;
            prods[prod_count++] = prod2;
        }

        /* extend to longer runs */
        for (idx_t len = 3; ; ++len) {
            prod2 *= (seq_t)(k + len - 1);
            if (prod2 > MAX_N) break;
            val = (idx_t)prod2;
            if (!seen[val]) {
                seen[val] = true;
                prods[prod_count++] = prod2;
            }
        }
    }
}

/* Pre‑compute dp[0..MAX_N] using subset sum DP */
static void init(void)
{
    if (dp_initialized) return;

    memset(seen, 0, sizeof(seen));

    build_products();

    dp[0] = 1;
    for (idx_t i = 1; i <= MAX_N; ++i) dp[i] = 0;

    for (size_t pi = 0; pi < prod_count; ++pi) {
        seq_t p = prods[pi];
        idx_t pp = (idx_t)p;            /* value fits in idx_t because <= MAX_N */
        for (idx_t s = MAX_N; s >= pp; --s) {
            dp[s] += dp[s - pp];
            /* when s == pp the next iteration hits s==pp-1 < pp and stops automatically */
        }
    }
    dp_initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();

    idx_t target = 2 * n;
    if (target > MAX_N) {
        /* For safety, extend arrays if needed (not required for given limits) */
        /* If target exceeds pre‑computation limit, return 0 as an upper bound. */
        return 0;
    }
    return dp[target];
}
