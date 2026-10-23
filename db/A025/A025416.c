/*
 * A025416  Least sum of 4 nonzero squares in exactly n ways.
 * https://oeis.org/A025416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1666, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least integer k such that the number of unordered
 * representations of k as a sum of four positive squares equals n.
 * The function computes this via dynamic programming over all sums
 * up to a chosen upper bound sufficient for n ≤ 10000.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_N 10000U          /* maximum n we are prepared to answer */
#define KMAX 300000U          /* upper bound for sums (sufficient for n≤10000) */

/* global memoisation array */
static seq_t min_k[MAX_N + 1] = {0};

/* initialise the table lazily on first call */
static void init(void)
{
    static bool ready = false;
    if (ready) return;
    ready = true;

    /* generate all positive squares up to KMAX */
    idx_t num_squares = 0;
    seq_t *squares = NULL;
    {
        /* first count them */
        for (idx_t i = 1;; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i;
            if (sq > (seq_t)KMAX) break;
            ++num_squares;
        }
        squares = (seq_t *)malloc(num_squares * sizeof(seq_t));
        idx_t idx = 0;
        for (idx_t i = 1;; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i;
            if (sq > (seq_t)KMAX) break;
            squares[idx++] = sq;
        }
    }

    /* dynamic programming table: dp[sum][k], k=0..4 */
    const size_t dp_rows = (size_t)KMAX + 1;
    uint16_t *dp = (uint16_t *)calloc(dp_rows * 5, sizeof(uint16_t));
    if (!dp) return;  /* allocation failure: return early */

    dp[0 * 5 + 0] = 1;  /* one way to make sum 0 with 0 squares */

    /* cycle over all squares (unlimited supply, unordered) */
    for (idx_t s_idx = 0; s_idx < num_squares; ++s_idx) {
        seq_t sq = squares[s_idx];
        if (sq > (seq_t)KMAX) break;               /* safety */
        idx_t sq_idx = (idx_t)sq;
        for (idx_t sum = sq_idx; sum <= KMAX; ++sum) {
            size_t base = sum * 5U;
            size_t pre_base = (sum - sq_idx) * 5U;
            for (idx_t k = 1; k <= 4; ++k) {
                uint16_t cur = dp[base + k];
                uint16_t add = dp[pre_base + k - 1];
                uint32_t val = (uint32_t)cur + (uint32_t)add;
                if (val > MAX_N + 1) val = MAX_N + 1;
                dp[base + k] = (uint16_t)val;
            }
        }
    }

    /* fill the min_k table with minimal sums for each count */
    min_k[0] = 0;  /* definition a(0) = 0 */
    for (idx_t sum = 1; sum <= KMAX; ++sum) {
        uint16_t cnt = dp[sum * 5U + 4];
        if (cnt == 0 || cnt > MAX_N) continue;   /* ignore zero or out‑of‑range counts */
        if (min_k[(idx_t)cnt] == 0) {
            min_k[(idx_t)cnt] = (seq_t)sum;
        }
    }

    free(dp);
    free(squares);
}

/* the exported function */
seq_t a(idx_t n)
{
    if (n > MAX_N) return 0;  /* out of range; caller should not request this */
    init();
    return min_k[(idx_t)n];
}
