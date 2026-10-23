/*
 * A146291  Triangle T(n,m) read by rows (n >= 1, 0 <= m <= A001222(n)),
 *          giving the number of divisors of n with m prime factors (counted
 *          with multiplicity).
 * https://oeis.org/A146291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(k,m) read by rows of the triangle T(k,m) = coefficient
 *            of k^m in ∏_p (1 + k + ... + k^{e_p}) where the product runs
 *            over the prime powers p^{e_p} of k. a(n) is the n‑th entry in
 *            this triangle when the rows are concatenated in order. The
 *            implementation computes the row index corresponding...
 * Range:     u8 n <= 9617, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include any necessary headers */
#include <stdint.h>

/* helper: factorise 'x' into exponent array.
   exps must have space for at least 16 elements.
   returns: number of distinct prime factors.  */
static int factor_exponents(idx_t x, int exps[16])
{
    idx_t m = x;
    int cnt = 0;
    int e;

    /* factor 2 */
    if ((m & 1ULL) == 0) {
        e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        exps[cnt++] = e;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    if (m > 1) {            /* remaining prime factor */
        exps[cnt++] = 1;
    }
    return cnt;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, though OEIS offset is 1 */

    /* --- find the row 'row' and within‑row position 'm' --- */
    idx_t rem = n;          /* remaining index to consume */
    idx_t row = 1;          /* current row number (k) */
    int exps[16];
    int ecnt;
    idx_t row_len;
    int bigOmega;

    while (1) {
        ecnt = factor_exponents(row, exps);
        bigOmega = 0;
        for (int i = 0; i < ecnt; ++i) bigOmega += exps[i];
        row_len = (idx_t)(bigOmega + 1);   /* length of current row */

        if (rem <= row_len) {
            /* we are in this row, position m = rem-1 */
            int m = (int)(rem - 1);

            /* --- compute T(row, m) by DP over exponents --- */
            /* maximum possible bigOmega is small for ordinary n,
               so we can use a fixed size array */
            seq_t dp[64] = {0};
            seq_t newdp[64] = {0};
            int curr_len = 0;
            dp[0] = 1;

            for (int idx = 0; idx < ecnt; ++idx) {
                int e = exps[idx];
                int new_len = curr_len + e;

                /* zero newdp up to new_len */
                for (int i = 0; i <= new_len; ++i) newdp[i] = 0;

                /* convolution: for each existing sum i, add i+0..i+e */
                for (int i = 0; i <= curr_len; ++i) {
                    seq_t val = dp[i];
                    if (val == 0) continue;
                    for (int j = 0; j <= e; ++j) {
                        newdp[i + j] += val;
                    }
                }

                /* copy back */
                for (int i = 0; i <= new_len; ++i) dp[i] = newdp[i];
                curr_len = new_len;
            }

            return dp[m];
        } else {
            rem -= row_len;
            ++row;
        }
    }
}
