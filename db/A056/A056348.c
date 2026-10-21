/*
 * A056348  Number of primitive (period n) bracelets using exactly two
 *          different colored beads.
 * https://oeis.org/A056348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * A056342(n/d) where A056342 gives the
 *            number of binary bracelets of length m
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, factor_u64, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mu_of.h"
#include "../utils/big_pow.h"

/* Power of two: 2^exp */
static inline seq_t pow2(idx_t exp) { return big_pow((seq_t)2, (seq_t)exp); }

/* φ(n) computed by prime factorisation */
static seq_t phi_of(idx_t n)
{
    if (n <= 1) return (seq_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t result = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i)
        result = result / (seq_t)primes[i] * ( (seq_t)primes[i] - 1 );
    return result;
}

/* Number of binary bracelets of length m (not necessarily primitive) */
static seq_t bracelet_count(idx_t m)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t d1 = i, d2 = m / i;
            seq_t phi1 = phi_of(d1);
            seq_t pow1 = pow2(m / d1);
            sum += phi1 * pow1;
            if (d2 != d1) {
                seq_t phi2 = phi_of(d2);
                seq_t pow2_ = pow2(m / d2);
                sum += phi2 * pow2_;
            }
        }
    }
    seq_t reflection = 0;
    if (m & 1) { /* odd */
        seq_t pow_term = pow2((m + 1) / 2);
        reflection = m * pow_term;
    } else {   /* even */
        seq_t half_pow = pow2(m / 2);
        seq_t half_plus_pow = pow2(m / 2 + 1);
        reflection = (m / 2) * (half_pow + half_plus_pow);
    }
    seq_t numerator = sum + reflection;
    return numerator / (2 * m);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;     /* no two different colors for a single bead */

    seq_t total = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i, d2 = n / i;
            int mu1 = mu_of(d1);
            if (mu1 != 0) {
                seq_t b1 = bracelet_count(n / d1);
                if (mu1 == 1) total += b1;
                else          total -= b1;
            }
            if (d2 != d1) {
                int mu2 = mu_of(d2);
                if (mu2 != 0) {
                    seq_t b2 = bracelet_count(n / d2);
                    if (mu2 == 1) total += b2;
                    else          total -= b2;
                }
            }
        }
    }
    return total;
}
