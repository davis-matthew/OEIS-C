/*
 * A061680  a(n) = gcd(d(n^2), d(n)).
 * https://oeis.org/A061680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(d(n^2), d(n)) where d(x) is the number of divisors of
 *            x
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

/* Lazily computed list of primes up to 1,000,000 (sufficient for n ≤ 10⁶²) */
static bool primes_init = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (!primes_init) {
        /* generate all primes ≤ 1,000,000 */
        build_primes_upto(1000000, &primes, &prime_cnt);
        primes_init = true;
    }
}

/* Compute a(n) = gcd(d(n^2), d(n)) */
seq_t a(idx_t n) {
    init_primes();

    uint64_t m = (uint64_t)n;       /* n itself */
    uint64_t m_sq = m;              /* working copy for factorisation, same as m */

    seq_t d_n  = 1;   /* d(n)   */
    seq_t d_n2 = 1;   /* d(n^2) */

    /* Factor n and compute divisor counts */
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            /* Count exponent of p in n */
            uint64_t exp = 0;
            while (m % p == 0) {
                m   /= p;
                exp += 1;
            }
            d_n  *= (exp + 1);          /* (e+1) for d(n)  */
            d_n2 *= (exp * 2 + 1);      /* (2e+1) for d(n^2) */
        }
    }

    if (m > 1) { /* remaining prime factor */
        d_n  *= 2;   /* exponent 1 -> (1+1) = 2 */
        d_n2 *= 3;   /* (2*1+1) = 3 */
    }

    return gcd(d_n, d_n2);
}
