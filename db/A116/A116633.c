/*
 * A116633  Triangle read by rows: T(n,k) is the number of partitions of n
 *          having exactly k parts that are multiple of 3 (n>=0,
 *          0<=k<=floor(n/3)).
 * https://oeis.org/A116633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Output a(n) is the flattened triangle of A116633, i.e. a(n) =
 *            T(r,k) where r and k satisfy n = row_start[r] + k, 0 ≤ k ≤
 *            floor(r/3), and row_start[0] = 0, row_start[r] = row_start[r-1]
 *            + 1 + floor((r-1)/3)
 * Range:     u8 n <= 84, u16 n <= 495, u32 n <= 3728, u64 n <= 8887,
 *            u128 n <= 8887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include standard headers that are safe for both C11 and C++17 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_N    500                  /* maximum row index (≥ needed for 10 000 flattened terms) */
#define MAX_K    (MAX_N / 3)          /* maximum k value that can occur */

/* helper for minimum */
static inline idx_t min_idx(idx_t a, idx_t b) { return (a < b) ? a : b; }

/* static tables and a flag indicating initialization */
static seq_t dp[MAX_N + 1][MAX_K + 1];
static idx_t row_start[MAX_N + 1];
static bool initialized = false;

/* one‑time initialization routine */
static void init_tables(void)
{
    /* zero all entries */
    for (idx_t i = 0; i <= MAX_N; ++i)
        for (idx_t j = 0; j <= MAX_K; ++j)
            dp[i][j] = (seq_t)0;

    dp[0][0] = (seq_t)1;

    /* dynamic programming over all part sizes */
    for (idx_t s = 1; s <= MAX_N; ++s)
    {
        bool mult3 = (s % 3 == 0);
        for (idx_t n = s; n <= MAX_N; ++n)
        {
            /* only k up to floor(n/3) can be non‑zero for dp[n][k] */
            idx_t kmax = n / 3;
            for (idx_t k = 0; k <= kmax; ++k)
            {
                if (mult3)
                {
                    if (k > 0)
                        dp[n][k] += dp[n - s][k - 1];
                }
                else
                {
                    dp[n][k] += dp[n - s][k];
                }
            }
        }
    }

    /* precompute row_start array */
    row_start[0] = 0;
    for (idx_t r = 1; r <= MAX_N; ++r)
    {
        idx_t prev = row_start[r - 1];
        idx_t len_prev = 1 + ((r - 1) / 3);
        row_start[r] = prev + len_prev;
    }
}

/* public function required by the build system */
seq_t a(idx_t index)
{
    if (!initialized)
    {
        init_tables();
        initialized = true;
    }

    /* locate the row r that contains the index */
    idx_t r = 0;
    while (r <= MAX_N)
    {
        idx_t len = 1 + (r / 3);
        if (index < row_start[r] + len)
            break;
        ++r;
    }

    /* safety: if r > MAX_N the index was too large for the precomputation */
    if (r > MAX_N)
    {
        /* return 0 (should not happen for the intended input range) */
        return (seq_t)0;
    }

    idx_t k = index - row_start[r];
    return dp[r][k];
}
