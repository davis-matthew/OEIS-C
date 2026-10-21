/*
 * A067911  Product of gcd(k,n) for 1 <= k <= n.
 * https://oeis.org/A067911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{d|n} d^{φ(n/d)} The implementation enumerates
 *            all divisors of n using the prime factorisation of n and
 *            multiplies each divisor to the power of φ(n/d). φ(m) is computed
 *            from the same prime data. The exponent φ(m) always fits in idx_t
 *            (since φ(m) ≤ m ≤ n ≤ 2^64‑1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 23, u64 n <= 47, u128 n <= 83,
 *            big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

/* fast power for seq_t base raised to an idx_t exponent */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* integer power for idx_t base with small exponent (used to build φ) */
static idx_t pow_uint(uint64_t base, int exp) {
    idx_t res = (idx_t)1;
    while (exp--) res *= (idx_t)base;
    return res;
}

/* recursive enumeration of divisors */
static void dfs_divisors(int pos, seq_t cur, int *cur_exp,
                         int cnt, uint64_t *pr, int *exp,
                         seq_t *prod) {
    if (pos == cnt) {
        /* compute φ(n/cur) from the current exponents */
        idx_t phi = (idx_t)1;
        for (int i = 0; i < cnt; ++i) {
            int rem = exp[i] - cur_exp[i];
            if (rem > 0) {
                phi *= pow_uint(pr[i], rem - 1) * (pr[i] - 1);
            }
        }
        *prod *= pow_seq(cur, phi);
        return;
    }
    /* iterate exponent of current prime */
    seq_t val = cur;
    for (int e = 0; e <= exp[pos]; ++e) {
        cur_exp[pos] = e;
        dfs_divisors(pos + 1, val, cur_exp,
                     cnt, pr, exp, prod);
        if (e < exp[pos]) val *= (seq_t)pr[pos];
    }
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0; /* undefined for n=0 */
    uint64_t primes[20];
    int  exps[20];
    int  cnt;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    int  cur_exp[20] = {0};
    seq_t prod = (seq_t)1;
    dfs_divisors(0, (seq_t)1, cur_exp,
                 cnt, primes, exps, &prod);
    return prod;
}
