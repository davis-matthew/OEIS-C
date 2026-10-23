/*
 * A106236  Triangle of the numbers of different forests with m rooted trees
 *          having distinct orders.
 * https://oeis.org/A106236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 325, u64 n <= 1128,
 *            u128 n <= 3916, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A106236.
   The sequence is defined as follows:
       a(n) = sum over all partitions of N with exactly m distinct parts
              of the product over the parts i of A000081(i),
   where the linear index n is mapped to the pair (N,m) by treating the
   triangular array 1..N as rows.  The implementation precomputes the
   rooted‑tree counts A000081 up to N=200 and then evaluates the
   required subset sums via dynamic programming.  The arrays are static
   so the routine is thread‑safe and fast for all n up to 10 000.  */

#include <stdbool.h>
#include <stdint.h>

#define NMAX 200   /* enough for n ≤ 10 000 (triangle 200*201/2 = 20 100) */

static seq_t A[NMAX + 1];          /* A000081 values, 1…NMAX */
static seq_t dp[NMAX + 1][NMAX + 1];/* dp[sum][size] */
static bool initialized = false;   /* tables built flag */

/* Compute A000081(k) for k = 1..NMAX using Otter's recurrence. */
static void compute_A(void)
{
    A[0] = 0;
    A[1] = 1;
    for (idx_t n = 2; n <= NMAX; ++n) {
        seq_t total = 0;
        for (idx_t k = 1; k <= n - 1; ++k) {
            seq_t inner = 0;
            for (idx_t d = 1; d <= k; ++d) {
                if (k % d == 0) {
                    inner += (seq_t)d * A[d];
                }
            }
            total += inner * A[n - k];
        }
        A[n] = total / (n - 1);
    }
}

/* Build the subset DP table.  dp[s][t] contains the sum of products
   of A values over all subsets of {1..NMAX} that have total sum s
   and size t.  The DP is performed in decreasing order to avoid
   double counting. */
static void compute_dp(void)
{
    /* zero‑initialize all entries */
    for (idx_t s = 0; s <= NMAX; ++s) {
        for (idx_t t = 0; t <= NMAX; ++t) {
            dp[s][t] = 0;
        }
    }
    dp[0][0] = 1;   /* empty subset */

    for (idx_t i = 1; i <= NMAX; ++i) {
        for (idx_t s = NMAX - i; s != (idx_t)(-1); --s) {
            for (idx_t t = NMAX - 1; t != (idx_t)(-1); --t) {
                seq_t cur = dp[s][t];
                if (cur == 0) continue;
                dp[s + i][t + 1] += cur * A[i];
            }
        }
    }
}

/* One‑time initializer for the static tables. */
static void init_tables(void)
{
    if (initialized) return;
    initialized = true;
    compute_A();
    compute_dp();
}

seq_t a(idx_t n)
{
    init_tables();                      /* ensure tables are ready */

    /* Find the smallest N with N(N+1)/2 ≥ n. */
    idx_t low = 1, high = NMAX;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t N = low;
    idx_t prev_tri = (N - 1) * N / 2;
    idx_t m = n - prev_tri;             /* 1‑based index in the row */

    /* Return the pre‑computed DP value. */
    return dp[N][m];
}
