/*
 * A144890  Partition number array, called M31hat(5).
 * https://oeis.org/A144890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = flattened array of a(n,k) from A144890
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"

#define MAX_R 200

static uint64_t dp[MAX_R + 1][MAX_R + 1][MAX_R + 1];
static uint64_t p_exact[MAX_R + 1][MAX_R + 1];
static uint64_t cum[MAX_R + 1];
static seq_t C[MAX_R + 1];
static bool initialized = false;

static void init_arrays(void) {
    if (initialized) return;

    /* base case: only the empty partition */
    for (int m = 0; m <= MAX_R; ++m)
        dp[0][0][m] = 1;

    /* dp[n][k][0] = 0 except dp[0][0][0] = 1 */
    for (int n = 0; n <= MAX_R; ++n) {
        for (int k = 0; k <= n; ++k) {
            dp[n][k][0] = (n == 0 && k == 0) ? 1 : 0;
        }
    }

    /* recurrence: dp[n][k][m] = dp[n][k][m-1] + dp[n-m][k-1][m] */
    for (int m = 1; m <= MAX_R; ++m) {
        for (int n = 0; n <= MAX_R; ++n) {
            for (int k = 0; k <= n; ++k) {
                uint64_t val = dp[n][k][m - 1];
                if (n >= m && k >= 1)
                    val += dp[n - m][k - 1][m];
                dp[n][k][m] = val;
            }
        }
    }

    /* exact-partition counts */
    for (int n = 1; n <= MAX_R; ++n) {
        for (int k = 1; k <= n; ++k) {
            p_exact[n][k] = dp[n][k][n];   /* all partitions of n into k parts */
        }
    }

    /* cumulative sums of partition numbers */
    cum[0] = 0;
    for (int n = 1; n <= MAX_R; ++n) {
        uint64_t sum = 0;
        for (int k = 1; k <= n; ++k) sum += p_exact[n][k];
        cum[n] = cum[n - 1] + sum;
    }

    /* pre‑compute C[j] = (j+3)! / 24  for all j up to MAX_R */
    for (int j = 1; j <= MAX_R; ++j) {
        seq_t fact = factorial((idx_t)(j + 3));
        C[j] = fact / (seq_t)24;
    }

    initialized = true;
}

seq_t a(idx_t n) {
    init_arrays();

    /* locate the row r such that cum[r-1] < n <= cum[r] */
    int r = 1;
    while (r <= MAX_R && cum[r] < n) ++r;
    if (r > MAX_R) return (seq_t)0;            /* unreachable for n <= 10000 */

    uint64_t k_within_row = n - cum[r - 1];     /* 1‑based index within row r */

    /* locate the exact partition length L */
    uint64_t remain = k_within_row;
    int L;
    for (L = 1; L <= r; ++L) {
        uint64_t cnt = p_exact[r][L];
        if (remain <= cnt) break;
        remain -= cnt;
    }

    /* unrank the partition of length L */
    int parts[L];
    int rem_sum = r;
    int rem_parts = L;
    int prev = r;
    uint64_t pos = remain;                     /* 1‑based within current L */

    for (int i = 0; i < L; ++i) {
        int max_val = rem_sum - (rem_parts - 1);
        if (max_val > prev) max_val = prev;

        for (int x = max_val; x >= 1; --x) {
            uint64_t cnt = dp[rem_sum - x][rem_parts - 1][x];
            if (pos <= cnt) {
                parts[i] = x;
                rem_sum -= x;
                rem_parts -= 1;
                prev = x;
                break;
            } else {
                pos -= cnt;
            }
        }
    }

    /* compute the product of |S1(5;j,1)| for each part j */
    seq_t ans = 1;
    for (int i = 0; i < L; ++i) ans *= C[parts[i]];

    return ans;
}
