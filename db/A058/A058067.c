/*
 * A058067  Number of polynomial functions from Z to Z/nZ.
 * https://oeis.org/A058067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} n / gcd(n, k!) (OEIS A058067)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special cases */
    if (n <= 1)
        return (seq_t)1;

    /* Factor n into primes p^e */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;

    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            e++;
            m /= 2;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        cnt++;
    }

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            cnt++;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt]   = 1;
        cnt++;
    }

    seq_t result = (seq_t)1;

    /* For each prime factor compute its contribution to the final product */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute sum_{k=1}^{n-1} min(e, v_p(k!)) */
        uint64_t sum_min = 0;
        uint64_t v = 0;          /* current exponent of p in k! */
        for (uint64_t k = 1; k < n; ++k) {
            uint64_t tmp = k;
            while (tmp % p == 0) {
                v++;
                tmp /= p;
            }
            sum_min += (v < e) ? v : e;
        }

        /* Exponent of p in a(n) */
        uint64_t exp_p = (uint64_t)n * e - sum_min;

        /* Multiply the result by p^exp_p */
        seq_t mult = big_pow((seq_t)p, (seq_t)exp_p);
        result *= mult;
    }

    return result;
}
