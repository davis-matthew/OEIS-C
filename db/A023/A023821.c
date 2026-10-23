/*
 * A023821  Sum of exponents in prime-power factorization of C(3n,n-2).
 * https://oeis.org/A023821
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents in the prime‑power factorization of
 *            C(3n, n-2)
 * Range:     u8 n <= 855, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          i.e. a(n) = bigomega( binom(3n, n-2) ) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

/* Helper to compute exponent of prime p in k! */
static seq_t vp(idx_t k, idx_t p)
{
    seq_t sum = 0;
    if (k == 0) return 0;
    idx_t pow = p;
    while (pow <= k) {
        sum += k / pow;
        /* avoid overflow in pow *= p */
        if (pow > k / p) break;
        pow *= p;
    }
    return sum;
}

/* Cache for primes up to a current limit */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* sequence defined for n ≥ 2 */

    /* Needed limit for primes: all primes ≤ 3n */
    idx_t limit = 3 * n;
    if (limit > primes_limit) {
        /* re‑sieve higher limit */
        free(primes);                  /* discard old array if any */
        sieve_primes(limit, &primes, &primes_cnt);
        primes_limit = limit;
    }

    idx_t N3  = 3 * n;        /* 3n          */
    idx_t N2  = n - 2;        /* n-2         */
    idx_t N23 = 2 * n + 2;    /* 2n+2        */

    seq_t result = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > N3) break;          /* primes above 3n contribute 0 */

        seq_t e = vp(N3, p) - vp(N2, p) - vp(N23, p);
        if (e > 0) result += e;
    }

    return result;
}
