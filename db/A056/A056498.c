/*
 * A056498  Number of primitive (period n) periodic palindromes using exactly
 *          two different symbols.
 * https://oeis.org/A056498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * A027383(n/d - 2) (A027383(-1)=0)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static int mobius(idx_t n) {
    if (n == 1) return 1;
    int cnt = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                if (exp > 1) return 0;          /* square factor → μ=0 */
            }
            ++cnt;
        }
    }
    if (tmp > 1) ++cnt;                           /* remaining prime */
    return (cnt % 2 == 0) ? 1 : -1;
}

static seq_t A027383(idx_t k) {
    if ((int64_t)k < 0) return 0;
    if (k == 0) return 1;
    if ((k & 1) == 0) {                           /* k even: k = 2m */
        idx_t m = k >> 1;
        seq_t term = pow2(m + 1) + pow2(m) - (seq_t)2;
        return term;
    } else {                                      /* k odd: k = 2m+1 */
        idx_t m = (k - 1) >> 1;
        seq_t term = pow2(m + 2) - (seq_t)2;
        return term;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                         /* below offset */
    seq_t pos = 0, neg = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i, d2 = n / i;
            int mu1 = mobius(d1);
            if (mu1 != 0) {
                idx_t m = n / d1 - 2;
                if (m >= 0) {
                    seq_t val = A027383(m);
                    if (mu1 > 0) pos += val; else neg += val;
                }
            }
            if (d2 != d1) {
                int mu2 = mobius(d2);
                if (mu2 != 0) {
                    idx_t m = n / d2 - 2;
                    if (m >= 0) {
                        seq_t val = A027383(m);
                        if (mu2 > 0) pos += val; else neg += val;
                    }
                }
            }
        }
    }
    return pos - neg;
}
