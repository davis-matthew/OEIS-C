/*
 * A007791  From a problem concerning circulant matrices and Gauss sums.
 * https://oeis.org/A007791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, more
 * Formula:   multiplicative with a(n) = f(n+1), f(m) = prod f(p^k) over p^k
 *            || m, where f(p^k) = #{x in Z[zeta_5]/(p^k) : p^k | Norm(x)} for
 *            p != 5 (p splits into 4/f primes of residue degree f =
 *            ord_5(p)), f(5) = 0, f(5^k) = 5^(4k-1) for k >= 2
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t ipow_s(seq_t b, idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= b;
        b *= b;
        e >>= 1;
    }
    return r;
}

static seq_t local_count(uint64_t p, idx_t k)
{
    idx_t f, g, K, v, s, i;
    seq_t q, c[70], dp[70], nd[70];
    if (p == 5) {
        if (k == 1) return 0;
        return ipow_s((seq_t)5, 4 * k - 1);
    }
    f = (p % 5 == 1) ? 1 : (p % 5 == 4) ? 2 : 4;
    g = 4 / f;
    K = (k + f - 1) / f;
    q = ipow_s((seq_t)p, f);
    for (v = 0; v <= k; v++) {
        if (v < k) c[v] = (q - 1) * ipow_s(q, k - v - 1);
        else c[v] = 1;
    }
    for (s = 0; s <= K; s++) dp[s] = 0;
    dp[0] = 1;
    for (i = 0; i < g; i++) {
        for (s = 0; s <= K; s++) nd[s] = 0;
        for (s = 0; s <= K; s++) {
            for (v = 0; v <= k; v++) {
                idx_t t = s + v;
                if (t > K) t = K;
                nd[t] += dp[s] * c[v];
            }
        }
        for (s = 0; s <= K; s++) dp[s] = nd[s];
    }
    return dp[K];
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n + 1;
    seq_t r = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t k = 0;
            while (m % p == 0) { m /= p; k++; }
            r *= local_count(p, k);
        }
    }
    if (m > 1) r *= local_count(m, 1);
    return r;
}
