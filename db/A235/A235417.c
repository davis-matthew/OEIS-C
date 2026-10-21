/*
 * A235417  Number of (n+1)X(1+1) 0..2 arrays with 2X2 subblock sum of squares
 *          lexicographically nondecreasing rowwise and nonincreasing
 *          columnwise.
 * https://oeis.org/A235417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 41, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (derived):
    a(n) = C(⌈(n+1)/2⌉) * C(⌊(n+1)/2⌋)

    For each row the sum of squares of its two entries can be
    0,1,2,4,5,8, occurring respectively in 1,2,1,2,2,1 patterns.
    For an (n+1)×2 array the constraint reduces to the sums on
    even‑indexed rows being non‑decreasing and independently the
    sums on odd‑indexed rows being non‑decreasing.
    Hence a(n) is the product of the weighted counts of non‑decreasing
    sequences of length k over the six square‑sum values; the weight of a
    value v is the number of patterns having that sum.  The coefficient
    of x^k in ∏_v 1/(1‑w_v x) gives the required count C(k).              */

#include <stddef.h>
#include <stdbool.h>

/* Helper: pre‑compute C(k) for k = 1 … 5001 (max for n ≤ 10000) */
static seq_t C_even[5002];
static bool precomp_done = false;

static void precompute(void)
{
    /* frequencies of row square sums: 0,1,2,4,5,8 */
    const seq_t freq[6] = { (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)1 };
    seq_t dp_prev[6], dp_next[6];

    /* initial length 1 */
    for (int i = 0; i < 6; ++i)
        dp_prev[i] = freq[i];

    C_even[1] = dp_prev[0] + dp_prev[1] + dp_prev[2] +
                dp_prev[3] + dp_prev[4] + dp_prev[5];

    const size_t MAX_K = 5001;
    for (size_t len = 2; len <= MAX_K; ++len) {
        seq_t cumulative = 0;
        seq_t total = 0;
        for (int i = 0; i < 6; ++i) {
            cumulative += dp_prev[i];
            dp_next[i] = freq[i] * cumulative;
            total += dp_next[i];
        }
        C_even[len] = total;
        /* prepare for next iteration */
        for (int i = 0; i < 6; ++i)
            dp_prev[i] = dp_next[i];
    }
}

/* Public function required by the benchmarking harness */
seq_t a(idx_t n)
{
    if (!precomp_done) {
        precompute();
        precomp_done = true;
    }
    if (n < 1) return 0;

    /* number of rows in each parity class */
    idx_t n_even = (n + 2) / 2;    /* ceil((n+1)/2) */
    idx_t n_odd  = (n + 1) / 2;    /* floor((n+1)/2) */

    /* n_even and n_odd are bounded by 5001 for n ≤ 10000 */
    seq_t res = C_even[n_even];
    res = res * C_even[n_odd];
    return res;
}
