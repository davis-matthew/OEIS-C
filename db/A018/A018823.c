/*
 * A018823  n is the sum of k nonzero squares for all 4 <= k <= n-14.
 * https://oeis.org/A018823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer N such that N can be expressed as a sum
 *            of k * nonzero squares for all 4 ≤ k ≤ N-14. (OEIS A018823)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_M 20000                          /* upper bound for searched numbers */
#define WORDLEN ((MAX_M + 63) / 64)          /* number of 64‑bit words per bitset */

static unsigned long long *dp_tbl = NULL;   /* DP table: dp_tbl[m*WORDLEN + w] */
static seq_t seq_vals[20000];               /* cache of sequence terms */
static size_t seq_len = 0;

/* Allocate and compute the DP table once. */
static void init(void)
{
    if (dp_tbl != NULL) return;             /* already initialized */

    /* Allocate DP table */
    dp_tbl = (unsigned long long *)malloc((MAX_M + 1) * WORDLEN * sizeof(unsigned long long));
    if (!dp_tbl) return;                    /* allocation failed (unlikely) */
    memset(dp_tbl, 0, (MAX_M + 1) * WORDLEN * sizeof(unsigned long long));

    /* dp[0] has bit 0 set (0 represented with 0 squares) */
    dp_tbl[0] = 1ULL;

    /* Unbounded knapsack: add each square value as a coin */
    for (int sq = 1; sq * sq <= MAX_M; ++sq) {
        int s = sq * sq;
        for (int n = s; n <= MAX_M; ++n) {
            unsigned long long *dst = dp_tbl + n * WORDLEN;
            const unsigned long long *src = dp_tbl + (n - s) * WORDLEN;
            unsigned long long carry = 0;
            for (int w = 0; w < WORDLEN; ++w) {
                unsigned long long new_carry = src[w] >> 63;
                unsigned long long shifted = (src[w] << 1) | carry;
                dst[w] |= shifted;
                carry = new_carry;
            }
        }
    }

    /* Build the sequence cache from the DP table */
    for (int n = 18; n <= MAX_M; ++n) {     /* the first qualifying number is 18 */
        int max_k = n - 14;
        int ok = 1;
        for (int k = 4; k <= max_k; ++k) {
            if (!(dp_tbl[n * WORDLEN + (k >> 6)] & (1ULL << (k & 63)))) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            if (seq_len < 20000) seq_vals[seq_len++] = (seq_t)n;
        }
    }
}

seq_t a(idx_t n)
{
    /* First call triggers initialization */
    static int initialized = 0;
    if (!initialized) {
        init();
        initialized = 1;
    }

    if (n == 0 || n > seq_len) return 0;    /* out of range */
    return seq_vals[n - 1];
}
