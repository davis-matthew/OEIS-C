/*
 * A173751  a(n) = gcd(n, lcm_{p is a prime divisor of n} (p-1)) = gcd(n,
 *          A173614(n)).
 * https://oeis.org/A173751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( n, lcm{ p-1 | p is a prime divisor of n } )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the desired value for any n ≥ 1.
 * It factors n by trial division, keeps the distinct prime factors and
 * their multiplicities, finds the maximum exponent of each such prime
 * occurring in any (p_i-1), and finally returns the product of each prime
 * raised to the minimum of its exponent in n and that maximum. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 */
    if (n == 1U)
        return (seq_t)1;

    /* Local arrays: at most a few distinct primes for n <= 10^7. */
    const int MAXP = 32;          /* sufficient for our limits */
    uint64_t primes[MAXP];
    int       expn [MAXP];        /* exponent of prime in n */
    int       lcm_exp[MAXP];      /* max exponent of prime in any (p-1) */
    int       cnt = 0;            /* number of distinct primes */

    /* --------------------------------------------------- */
    /* 1. Factor n, store primes and exponents             */
    /* --------------------------------------------------- */
    uint64_t temp = (uint64_t)n;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            int e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            primes[cnt] = d;
            expn [cnt] = e;
            lcm_exp[cnt] = 0;    /* initialise max exponent to 0 */
            ++cnt;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        primes[cnt] = temp;
        expn [cnt] = 1;
        lcm_exp[cnt] = 0;
        ++cnt;
    }

    /* --------------------------------------------------- */
    /* 2. For each prime p_i of n, factor (p_i-1) and
     *    update lcm_exp for primes that also divide n.   */
    /* --------------------------------------------------- */
    for (int i = 0; i < cnt; ++i) {
        uint64_t v = primes[i] - 1U;          /* p_i - 1   */
        uint64_t w = v;
        for (uint64_t d = 2; d * d <= w; ++d) {
            if (w % d == 0) {
                int e = 0;
                while (w % d == 0) {
                    w /= d;
                    ++e;
                }
                /* does d match any prime in primes[] ? */
                for (int j = 0; j < cnt; ++j) {
                    if (primes[j] == d) {
                        if (e > lcm_exp[j])
                            lcm_exp[j] = e;
                        break;
                    }
                }
            }
        }
        if (w > 1) {                          /* remaining prime factor */
            uint64_t d = w;
            for (int j = 0; j < cnt; ++j) {
                if (primes[j] == d) {
                    if (1 > lcm_exp[j])
                        lcm_exp[j] = 1;
                    break;
                }
            }
        }
    }

    /* --------------------------------------------------- */
    /* 3. Build the result: product of p_i^{min(expn[i], lcm_exp[i])} */
    /* --------------------------------------------------- */
    seq_t res = (seq_t)1;
    for (int i = 0; i < cnt; ++i) {
        int use = expn[i] < lcm_exp[i] ? expn[i] : lcm_exp[i];
        for (int k = 0; k < use; ++k)
            res = res * (seq_t)primes[i];
    }

    return res;
}
