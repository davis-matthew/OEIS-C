/*
 * A178101  Cardinality of the set of d, 2<=d<=n/2, which divide
 *          binomial(n-d-1,d-1) and are not coprime to n.
 * https://oeis.org/A178101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 476, u16 n <= 9988, u32 n <= 9988, u64 n <= 9988,
 *            u128 n <= 9988, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: a(n) = |{ d | 2 ≤ d ≤ n/2 , d | binomial(n‑d‑1, d‑1) and gcd(n,d) > 1 }| */
/* This implementation uses prime factorisation of d and the Legendre
   exponent formula to test divisibility by d.  It is O(n log n) and
   works for any n ≥ 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* exponent of prime p in m! (Legendre's formula) */
static inline uint64_t exp_factorial(uint64_t m, uint64_t p)
{
    uint64_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* no d satisfies the condition */
    seq_t cnt = 0;                 /* result to be returned */

    /* iterate over all d in [2, floor(n/2)] */
    for (idx_t d = 2; d <= n / 2; ++d) {
        /* skip d coprime to n */
        seq_t g = gcd((seq_t)n, (seq_t)d);
        if (g <= 1) continue;

        /* factor d */
        uint64_t primes[32];
        uint64_t exps[32];
        size_t pcnt = 0;
        factor_u64((uint64_t)d, primes, exps, &pcnt);

        /* prepare parameters for Legendre */
        uint64_t a = n - d - 1;      /* n-d-1  */
        uint64_t r = d - 1;          /* d-1 */
        uint64_t m3 = n - 2 * d;     /* n-2d */

        bool ok = true;
        for (size_t i = 0; i < pcnt && ok; ++i) {
            uint64_t p   = primes[i];
            uint64_t need = exps[i];   /* exponent of p in d */

            uint64_t exp_bin =
                exp_factorial(a, p) -
                exp_factorial(r, p) -
                exp_factorial(m3, p);

            if (exp_bin < need) ok = false;
        }

        if (ok) ++cnt;
    }

    return cnt;
}
