/*
 * A126262  a(n) is the denominator of the sum of the reciprocals of the
 *          positive integers k, k<=n, where every positive integer <= k and
 *          coprime to k is also coprime to n.
 * https://oeis.org/A126262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the sum of 1/k over all k ≤ n such that
 *            every integer i ≤ k that is coprime to k is also coprime to n.
 *            Equivalently, k must be divisible by every prime divisor of n
 *            that is ≤ k
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 28, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n into distinct primes (factor_u64 returns exponents as well) */
    uint64_t primes[64];
    uint64_t exps[64];          /* unused except for function signature */
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Start with the sum 0/1 */
    seq_t numer = (seq_t)0;
    seq_t denom = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        /* Check the property: k must be divisible by all prime divisors of n that are ≤ k */
        bool good = true;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            if ((idx_t)p > k) break;
            if (k % p != 0) { good = false; break; }
        }
        if (!good) continue;

        /* Add 1/k to the current fraction */
        seq_t k_seq = (seq_t)k;
        seq_t g = gcd(denom, k_seq);
        seq_t dk = denom / g;
        seq_t kk = k_seq / g;

        seq_t new_num = numer * kk + dk;
        seq_t new_den = dk * k_seq;

        seq_t g2 = gcd(new_num, new_den);
        numer = new_num / g2;
        denom = new_den / g2;
    }
    return denom;
}
