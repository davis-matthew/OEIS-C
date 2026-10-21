/*
 * A213280  Triangle read by rows: T(n,k) (n>=1, 1 <= k <= n) = number of
 *          permutations of [1..n] in which none of the cycle lengths are
 *          divisible by k.
 * https://oeis.org/A213280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = number of permutations of [1..n] in which none of the
 *            cycle lengths are divisible by k. The sequence a(n) is formed by
 *            reading this triangle row‑wise: a(1)=T(1,1), a(2)=T(2,1),
 *            a(3)=T(2,2), etc
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 82, u64 n <= 212,
 *            u128 n <= 596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static idx_t max_computed = 0;
static seq_t *fact = NULL;          /* factorials */
static seq_t **dp = NULL;           /* dp[n][k] : T(n,k) for 1 <= k <= n */

/* Compute factorials and dp values up to row r */
static void ensure_computed(idx_t r)
{
    if (r <= max_computed) return;

    /* enlarge factorial array */
    fact = (seq_t*)realloc(fact, (r + 1) * sizeof(seq_t));
    if (!fact) return;
    fact[0] = (seq_t)1;
    for (idx_t i = max_computed + 1; i <= r; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* enlarge dp array of row pointers */
    dp = (seq_t**)realloc(dp, (r + 1) * sizeof(seq_t*));
    if (!dp) return;

    /* compute new rows */
    for (idx_t i = max_computed + 1; i <= r; ++i) {
        dp[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!dp[i]) return;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t total = (seq_t)0;
            seq_t coeff_base = fact[i - 1];
            for (idx_t m = 1; m <= i; ++m) {
                if (m % k == 0) continue;
                idx_t left_n = i - m;
                seq_t left_val;
                if (left_n < k)
                    left_val = fact[left_n];      /* n! when n < k */
                else
                    left_val = dp[left_n][k];

                seq_t coef = coeff_base / fact[left_n];
                total += left_val * coef;
            }
            dp[i][k] = total;
        }
    }
    max_computed = r;
}

/* Convert global index n to triangle coordinates (row, col) */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* Solve r(r+1)/2 >= n */
    long double val = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < n) ++r;
    while ((r - 1) * r / 2 >= n) --r;
    *row = r;
    *col = n - (r - 1) * r / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but safe */
    idx_t r, k;
    index_to_row_col(n, &r, &k);
    ensure_computed(r);
    return dp[r][k];
}
