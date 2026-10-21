/*
 * A064355  Number of subsets of {1,2,..n} that sum to 1 mod n.
 * https://oeis.org/A064355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n, d odd} 2^(n/d) * μ(d) (μ = Möbius
 *            function)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* 2^exp computed using binary exponentiation.  Works for all rightful
 * seq_t types (unsigned integer or arbitrary precision class). */
static inline seq_t power_of_two(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Möbius function for idx_t arguments (up to the limits needed). */
static inline int mobius(idx_t n)
{
    if (n == 1) return 1;
    int mu = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt > 1) return 0;          /* squared prime factor → 0 */
            }
            mu = -mu;                           /* one distinct prime factor */
        }
    }
    if (temp > 1) mu = -mu;                    /* remaining prime factor > sqrt(n) */
    return mu;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n=0, safe return */

    seq_t sum = 0;

    /* iterate over all divisors; only odd divisors contribute */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        if (d1 & 1) {                         /* odd divisor d1 */
            int mu = mobius(d1);
            if (mu != 0) {
                seq_t term = power_of_two(n / d1);
                sum += (seq_t)mu * term;
            }
        }

        if (d2 != d1 && (d2 & 1)) {           /* distinct odd divisor d2 */
            int mu = mobius(d2);
            if (mu != 0) {
                seq_t term = power_of_two(n / d2);
                sum += (seq_t)mu * term;
            }
        }
    }

    /* division by n is exact */
    seq_t result = sum / (seq_t)n;
    return result;
}
