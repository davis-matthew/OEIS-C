/*
 * A181937  André numbers. Square array A(n,k), n>=2, k>=0, read by
 *          antidiagonals upwards, A(n,k) = n-alternating permutations of
 *          length k.
 * https://oeis.org/A181937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of n‑alternating permutations of length k, where
 *            (n,k) is obtained by reading the square array A(n,k) (n≥2,k≥0)
 *            by antidiagonals “upwards”. The sequence begins a(0)=1
 * Range:     u8 n <= 34, u16 n <= 76, u32 n <= 151, u64 n <= 349,
 *            u128 n <= 818, big any n
 * Validated: standard, pass, 94 terms through n = 8814 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Count n‑alternating permutations of length k.
   For pattern: comparisons p[i] < p[i+1] iff (i % n != 0), else >. */
static seq_t count_andre(int n, int k)
{
    if (k == 0)
        return (seq_t)1;

    /* dp[r] – number of permutations of current length with last element
       of rank r (1‑based).  Max k is < 200 for the required limits. */
    seq_t dp[200];
    seq_t ndp[200];
    seq_t pref[200];

    /* initialise for length 1 */
    for (int i = 0; i < 200; ++i) dp[i] = (seq_t)0;
    dp[1] = (seq_t)1;   /* one permutation of length 1 */

    for (int len = 1; len < k; ++len) {   /* current length = len, add element len+1 */
        int next = len + 1;
        /* prefix sums of dp[1..len] */
        pref[1] = dp[1];
        for (int i = 2; i <= len; ++i)
            pref[i] = pref[i-1] + dp[i];

        int less = (len % n != 0);   /* comparison between positions len and len+1 */
        if (less) {
            ndp[1] = (seq_t)0;
            for (int r = 2; r <= next; ++r)
                ndp[r] = pref[r-1];
        } else {
            seq_t total = pref[len];
            ndp[1] = total;
            for (int r = 2; r <= len; ++r)
                ndp[r] = total - pref[r-1];
            ndp[next] = (seq_t)0;
        }
        /* copy back */
        for (int i = 1; i <= next; ++i)
            dp[i] = ndp[i];
    }

    /* sum all dp[1..k] to obtain total permutations */
    seq_t total = (seq_t)0;
    for (int i = 1; i <= k; ++i)
        total += dp[i];
    return total;
}

idx_t _floor_log2_uint64(idx_t v); /* forward declaration if needed */

/* Main sequence function */
seq_t a(idx_t N)
{
    /* Map index N to (row=n, col=k) in the antidiagonal reading. */
    unsigned long long idx = N;
    long double val = sqrtl(1.0L + 8.0L * (long double)idx);
    unsigned long long d = (unsigned long long)((1.0L + val) / 2.0L) + 1ULL; /* diagonal number */

    unsigned long long prev = (d - 1ULL) * (d - 2ULL) / 2ULL;   /* cumulative count up to previous diagonal */
    unsigned long long pos  = idx - prev;                        /* offset within diagonal, 0‑based */

    unsigned long long n_row = d - pos;    /* n = row index */
    unsigned long long k_len = pos;        /* k = column index */

    return count_andre((int)n_row, (int)k_len);
}
