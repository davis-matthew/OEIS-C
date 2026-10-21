/*
 * A069220  Denominator of Sum_{1<=k<=n, gcd(k,n)=1} 1/k.
 * https://oeis.org/A069220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the sum of unit fractions 1/k for all 1 ≤
 *            k ≤ n with gcd(k,n)=1. The algorithm computes the lcm L of all
 *            such k, sums L/k, then divides L by the gcd of that sum and L.
 *            This yields the reduced denominator
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 28, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* defensive, offset starts at 1   */

    /* Generate all primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Compute L = lcm of all k with gcd(k,n)=1 */
    seq_t L = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        if ((seq_t)n % p == 0)            /* primes dividing n are omitted   */
            continue;

        /* Find the largest power of p not exceeding n */
        seq_t power = 1;
        while (power <= (seq_t)n / p)
            power *= p;

        L *= power;
    }

    /* Sum L/k over all coprime k: numerator of the sum in common denominator L */
    seq_t numer = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1)
            numer += L / (seq_t)k;
    }

    /* Reduce the fraction and return the denominator */
    seq_t g = gcd(numer, L);
    seq_t denom = L / g;

    free(primes);
    return denom;
}
