/*
 * A061598  Distance to the largest Catalan number which is less than, but
 *          divides the n-th Catalan number; i.e., a(n) gives minimum k (> 0)
 *          such that C_{n-k}|C_n.
 * https://oeis.org/A061598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that Catalan(n-k) divides Catalan(n).
 *            The function computes the prime‑factor exponents of Catalan(n)
 *            and walks backwards using the inverse recurrence C_{m-1} = C_m *
 *            (m+1) / (2*(2m-1)) at each step checking whether the current
 *            exponents are ≤ the original ones. The firs...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   factor_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_exp.h"

seq_t a(idx_t n)
{
    /* special case: for n = 1 the definition yields 1 */
    if (n <= 1) return (seq_t)1;

    /* 1.  Build smallest prime factor array up to 4*n */
    idx_t maxv = 4 * n;
    int *spf = (int *)calloc((size_t)(maxv + 1), sizeof(int));
    if (!spf) return (seq_t)0;

    for (idx_t i = 2; i <= maxv; ++i) {
        if (!spf[i]) {
            for (idx_t j = i; j <= maxv; j += i)
                if (!spf[j]) spf[(size_t)j] = (int)i;
        }
    }

    /* 2.  Build list of primes and mapping from prime to index */
    int *prime_idx = (int *)malloc((maxv + 1) * sizeof(int));
    if (!prime_idx) { free(spf); return (seq_t)0; }
    int primes_cnt = 0;
    for (int i = 2; i <= maxv; ++i) prime_idx[i] = -1;
    for (int i = 2; i <= maxv; ++i) {
        if (spf[i] == i) {
            prime_idx[i] = primes_cnt++;
        }
    }

    /* 3.  Compute prime‑factor exponents of C_n */
    long long *exp_n = (long long *)calloc((size_t)primes_cnt, sizeof(long long));
    if (!exp_n) { free(spf); free(prime_idx); return (seq_t)0; }
    for (idx_t i = 1; i <= n; ++i) {
        int num = 2 * (int)(2 * i - 1);   /* numerator in forward recurrence */
        factor_exp(spf, prime_idx, num, +1, exp_n);

        int den = (int)(i + 1);           /* denominator in forward recurrence */
        factor_exp(spf, prime_idx, den, -1, exp_n);
    }

    /* 4.  Walk backwards from n to 1 */
    long long *exp_cur = (long long *)calloc((size_t)primes_cnt, sizeof(long long));
    if (!exp_cur) { free(spf); free(prime_idx); free(exp_n); return (seq_t)0; }
    for (int i = 0; i < primes_cnt; ++i) exp_cur[i] = exp_n[i];

    for (idx_t m = n - 1; m >= 1; --m) {
        /* apply inverse recurrence: multiply by (m+2), divide by 2*(2m+1) */
        int numer = (int)(m + 2);                 /* m+2 */
        factor_exp(spf, prime_idx, numer, +1, exp_cur);

        int denom = 2 * (int)(2 * m + 1);         /* 2*(2m+1) */
        factor_exp(spf, prime_idx, denom, -1, exp_cur);

        /* check whether C_m divides C_n: exp_cur[p] <= exp_n[p] for all p */
        bool ok = true;
        for (int i = 0; i < primes_cnt; ++i) {
            if (exp_cur[i] > exp_n[i]) { ok = false; break; }
        }
        if (ok) {
            seq_t k = (seq_t)(n - m);
            free(spf); free(prime_idx); free(exp_n); free(exp_cur);
            return k;
        }
    }

    /* fallback (should not happen) */
    free(spf); free(prime_idx); free(exp_n); free(exp_cur);
    return (seq_t)1;
}
