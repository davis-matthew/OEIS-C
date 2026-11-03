/*
 * A072391  D2(n,n) = Sum_{1<=k<=n} (d_n(k^2)), where d_a(k^2)=card{d: d|k and
 *          1<=d<=a} for real a.
 * https://oeis.org/A072391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} #{ d | k^2 : d <= n }
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recursive generator of divisors of k^2 that are <= n and accumulate the count */
static void dfs_div_counts(int idx, seq_t prod, idx_t n,
                           int primes_cnt, const idx_t *primes,
                           const int *exps, seq_t *out) {
    if (idx == primes_cnt) {
        if (prod <= n) (*out)++;
        return;
    }
    idx_t p = primes[idx];
    seq_t cur = prod;
    for (int e = 0; e <= 2 * exps[idx]; ++e) {
        if (cur > n) break;
        dfs_div_counts(idx + 1, cur, n, primes_cnt, primes, exps, out);
        if (e == 2 * exps[idx]) break;
        cur *= p;
    }
}

/* Helper: factorize x into primes[] and exps[], returning number of distinct primes in cnt */
static void factorize(idx_t x, idx_t primes[], int exps[], int *cnt) {
    int c = 0;
    idx_t t = x;
    for (idx_t d = 2; d * d <= t; ++d) {
        if (t % d == 0) {
            int e = 0;
            while (t % d == 0) {
                t /= d;
                ++e;
            }
            primes[c] = d;
            exps[c] = e;
            ++c;
        }
    }
    if (t > 1) {               /* remaining prime factor */
        primes[c] = t;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t total = 0;
    /* Temporary buffers for factorization */
    idx_t primes[16];
    int exps[16];
    int cnt;

    for (idx_t k = 1; k <= n; ++k) {
        factorize(k, primes, exps, &cnt);
        seq_t cnt_k = 0;
        dfs_div_counts(0, (seq_t)1, n, cnt, primes, exps, &cnt_k);
        total += cnt_k;
    }
    return total;
}
