/*
 * A184286  Number of distinct n X 3 toroidal 0..4 arrays.
 * https://oeis.org/A184286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(3n)) * Σ_{d|n} φ(n/d) * (5^{3d} + 2·5^{d·gcd(n/d,3)})
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is a direct application of Burnside's lemma for the group C_n × C_3
   acting on n×3 arrays over the alphabet {0,1,2,3,4}.  */

#include "../utils/big_pow.h"
#include <stdint.h>

/* Euclidean algorithm for 64‑bit integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Euler's totient function – works for 64‑bit values */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the series starts at offset 1 */

    uint64_t nn = (uint64_t)n;          /* safe for factorization */

    seq_t S0 = 0;            /* sum over a of 5^{3gcd(n,a)}             */
    seq_t S1 = 0;            /* sum over a of 5^{gcd(n,a)*gcd(n/gcd(n,a),3)} */

    /* Enumerate all divisors d of n (gcd(n,a)=d) */
    uint64_t i;
    for (i = 1; i * i <= nn; ++i) {
        if (nn % i != 0) continue;
        uint64_t d1 = i;
        uint64_t d2 = nn / i;

        /* Process divisor d1 */
        {
            uint64_t d = d1;
            uint64_t m = nn / d;
            uint64_t phi_m = phi_uint64(m);
            uint64_t exp0 = 3 * d;
            uint64_t exp1 = d * gcd_uint64(m, 3ULL);

            seq_t pow0 = big_pow((seq_t)5, (seq_t)exp0);
            seq_t pow1 = big_pow((seq_t)5, (seq_t)exp1);

            S0 = S0 + pow0 * (seq_t)phi_m;
            S1 = S1 + pow1 * (seq_t)phi_m;
        }

        /* If d1 != d2, process divisor d2 */
        if (d1 != d2) {
            uint64_t d = d2;
            uint64_t m = nn / d;
            uint64_t phi_m = phi_uint64(m);
            uint64_t exp0 = 3 * d;
            uint64_t exp1 = d * gcd_uint64(m, 3ULL);

            seq_t pow0 = big_pow((seq_t)5, (seq_t)exp0);
            seq_t pow1 = big_pow((seq_t)5, (seq_t)exp1);

            S0 = S0 + pow0 * (seq_t)phi_m;
            S1 = S1 + pow1 * (seq_t)phi_m;
        }
    }

    /* Combine the sums: total = S0 + 2*S1 */
    seq_t total = S0 + S1 + S1;          /* S0 + 2*S1  */

    seq_t denom = (seq_t)(nn * 3ULL);    /* 3n */

    return total / denom;
}
