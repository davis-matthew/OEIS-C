/*
 * A023841  Sum of exponents in prime-power factorization of C(5n,n-4).
 * https://oeis.org/A023841
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑p [ e_p(5n!) – e_p((n‑4)!) – e_p((4n+4)!) ] * where
 *            e_p(m!) = ∑i≥1 ⌊m/p^i⌋ is the exponent of prime p in m!
 * Range:     u8 n <= 680, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, e_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#include "../utils/build_primes.h"
#include "../utils/e_factor.h"

/* static state for prime sieve */
static idx_t *prime_array = NULL;   /* array of primes up to current limit */
static size_t prime_count = 0;      /* number of primes in array          */
static idx_t prime_limit  = 0;      /* current sieve limit                */

/* ensure that primes up to `max` are available */
static void ensure_primes(idx_t max)
{
    if (max <= prime_limit)
        return;

    if (prime_array)
        free(prime_array);

    size_t cnt = 0;
    build_primes(max, &prime_array, &cnt);

    prime_count = cnt;
    prime_limit = max;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 4)                     /* below offset – behaviour irrelevant */
        return 0;

    idx_t m1 = 5 * n;              /* 5n!   */
    idx_t m2 = n - 4;              /* (n‑4)! */
    idx_t m3 = 4 * n + 4;          /* (4n+4)! */

    /* obtain all primes needed for this n */
    ensure_primes(m1);

    seq_t total = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_array[i];
        if (p > m1)                /* only primes ≤ 5n contribute */
            break;
        seq_t e = e_factor(m1, p)
                - e_factor(m2, p)
                - e_factor(m3, p);
        total += e;
    }
    return total;
}
