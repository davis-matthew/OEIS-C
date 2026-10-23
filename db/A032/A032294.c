/*
 * A032294  Number of aperiodic bracelets (turnover necklaces) with n beads of
 *          3 colors.
 * https://oeis.org/A032294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * B(n/d), where B(m) is the number of
 *            bracelets of length m with 3 colors. B(m) is computed using
 *            Burnside's lemma: if m is odd: B = ( Σ_{d|m} φ(d)·3^(m/d) +
 *            m·3^((m+1)/2) ) / (2m) if m is even: B = ( Σ_{d|m} φ(d)·3^(m/d)
 *            + (m/2)(3^(m/2+1)+3^(m/2)) ) / (2m)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"

static int mu_of(idx_t n) {
    if (n == 1) return 1;
    int cnt = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            if (tmp % (p * p) == 0) return 0;
            ++cnt;
            do { tmp /= p; } while (tmp % p == 0);
        }
    }
    if (tmp > 1) ++cnt;
    return (cnt % 2 == 0) ? 1 : -1;
}

static idx_t phi_of(idx_t n) {
    idx_t result = n;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            result -= result / p;
            do { tmp /= p; } while (tmp % p == 0);
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* Number of bracelets of length m with 3 colors */
static seq_t B_of(idx_t m) {
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;
            idx_t d2 = m / i;
            seq_t pow1 = seq_pow((seq_t)3, m / d1);
            sum += (seq_t)phi_of(d1) * pow1;
            if (d1 != d2) {
                seq_t pow2 = seq_pow((seq_t)3, m / d2);
                sum += (seq_t)phi_of(d2) * pow2;
            }
        }
    }
    seq_t extra;
    if (m % 2 == 1) {
        seq_t mid_pow = seq_pow((seq_t)3, (m + 1) / 2);
        extra = (seq_t)m * mid_pow;
    } else {
        seq_t half_pow_plus1 = seq_pow((seq_t)3, m / 2 + 1);
        seq_t half_pow = seq_pow((seq_t)3, m / 2);
        extra = (seq_t)(m / 2) * (half_pow_plus1 + half_pow);
    }
    seq_t total = sum + extra;
    seq_t denom = (seq_t)(2 * m);
    return total / denom;
}

seq_t a(idx_t n) {
    seq_t sum_pos = 0, sum_neg = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            int mu1 = mu_of(d1);
            if (mu1 == 1) sum_pos += B_of(n / d1);
            else if (mu1 == -1) sum_neg += B_of(n / d1);
            if (d1 != d2) {
                int mu2 = mu_of(d2);
                if (mu2 == 1) sum_pos += B_of(n / d2);
                else if (mu2 == -1) sum_neg += B_of(n / d2);
            }
        }
    }
    return sum_pos - sum_neg;
}
