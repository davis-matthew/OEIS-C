/*
 * A067593  Number of partitions of n into Lucas parts (A000032).
 * https://oeis.org/A067593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into Lucas parts (using
 *            unlimited copies of each Lucas number)
 * Range:     u8 n <= 22, u16 n <= 83, u32 n <= 459, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp      = NULL;   /* DP array: number of partitions for each n */
static idx_t dp_len   = 0;      /* allocated length of dp */

static idx_t *lucas   = NULL;   /* Lucas numbers (up to current limit) */
static idx_t lucas_size = 0;    /* number of Lucas numbers stored */
static idx_t lucas_cap  = 0;    /* capacity of lucas array */

/* Ensure lucas[] contains all Lucas numbers up to at least 'limit' */
static void ensure_lucas(idx_t limit) {
    if (lucas_size == 0) {
        lucas_cap = 16;
        lucas = (idx_t*)malloc(lucas_cap * sizeof(idx_t));
        lucas[lucas_size++] = (idx_t)2;      /* L0 = 2 */
        lucas[lucas_size++] = (idx_t)1;      /* L1 = 1 */
    }

    /* Generate more Lucas numbers until the last one exceeds limit */
    while (lucas[lucas_size - 1] <= limit) {
        idx_t next = lucas[lucas_size - 1] + lucas[lucas_size - 2];
        if (lucas_size == lucas_cap) {
            lucas_cap *= 2;
            lucas = (idx_t*)realloc(lucas, lucas_cap * sizeof(idx_t));
        }
        lucas[lucas_size++] = next;
    }
}

/* External linkage function requested by the test harness */
seq_t a(idx_t n) {
    /* Prepare the list of Lucas numbers and the DP array */
    ensure_lucas(n);
    if (dp_len <= n) {
        dp = (seq_t*)realloc(dp, (n + 1) * sizeof(seq_t));
        dp_len = n + 1;
    }

    /* Clear DP entries up to n */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;           /* empty partition counts as one way */

    /* Standard coin‑change DP: add partitions using each Lucas part */
    for (idx_t pi = 0; pi < lucas_size; ++pi) {
        idx_t v = lucas[pi];
        if (v > n) continue;    /* skip any Lucas number larger than n */
        for (idx_t j = v; j <= n; ++j)
            dp[j] += dp[j - v];
    }

    return dp[n];
}
