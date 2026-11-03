/*
 * A046643  From square root of Riemann zeta function: form Dirichlet series
 *          Sum b_n/n^s whose square is zeta function; sequence gives
 *          numerator of b_n.
 * https://oeis.org/A046643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, nice, mult
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 8989 (2026-11-01)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: A046643 is multiplicative with
 * a(1) = 1.
 * For each prime power p^e dividing n,
 *   let N = (2e-1)!!   (odd double factorial)
 *   let F = e!         (ordinary factorial)
 *   a(p^e) = N / gcd(N,F)
 * Then a(n) is the product of the a(p^e) over all prime powers of n.
 *
 * This computes the numerator of the rational b_n whose Dirichlet series
 * square equals ζ(s).  The computation uses only integer arithmetic and
 * the supplied utility functions.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

/* compute N = (2e-1)!!  and F = e!  (both fit in 64‑bits for e≤64) */
static inline __uint128_t odd_double_fact(uint64_t e, uint64_t *factOut)
{
    __uint128_t res = 1, fact = 1;
    for (uint64_t i = 1; i <= e; ++i) {
        res *= (2 * i - 1); /* odd double factorial */
        fact *= i;          /* ordinary factorial */
    }
    *factOut = (uint64_t)fact;   /* e! fits in 64‑bit for e≤64 */
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* n=0 is never queried */
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* obtain the prime factorisation of n */
    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t fact;
        __uint128_t N = odd_double_fact(e, &fact);  /* N = (2e-1)!! */

        /* reduce N by its common odd factors with e! */
        seq_t g   = gcd((seq_t)N, (seq_t)fact);
        seq_t local = (seq_t)(N / g);   /* a(p^e) */

        result *= local;
    }
    return result;
}
