/*
 * A023824  Sum of exponents in prime-power factorization of C(3n,n+2).
 * https://oeis.org/A023824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(C(3n, n+2)) = Σ_{p ≤ 3n} (E(3n,p) – E(n+2,p) –
 *            E(2n-2,p)), where E(m,p) is the exponent of prime p in m!
 * Range:     u8 n <= 859, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* static cache of primes */
static uint64_t *primes = NULL;   /* array of primes */
static size_t    prime_cnt = 0;   /* number of primes stored */
static idx_t     prime_lim = 0;   /* largest limit already generated */

/* ensures that primes[] contains all primes ≤ limit */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_lim >= limit)
        return;
    uint64_t *tmp = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &tmp, &cnt);         /* provided helper */
    if (primes) free(primes);                     /* discard old cache */
    primes = tmp;
    prime_cnt = cnt;
    prime_lim = limit;
}

/* exponent of prime p in m! */
static inline seq_t exp_in_factorial(idx_t m, uint64_t p)
{
    seq_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n<1; guard anyway */
        return (seq_t)0;
    idx_t limit = 3 * n;            /* primes needed up to 3n */
    ensure_primes_upto(limit);

    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > limit)
            break;
        seq_t e = exp_in_factorial(3 * n, p)
                - exp_in_factorial(n + 2, p)
                - exp_in_factorial(2 * n - 2, p);
        sum += e;
    }
    return sum;
}
