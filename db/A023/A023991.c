/*
 * A023991  Sum of exponents of primes in multinomial coefficient M(3n;
 *          n+1,n,n-1).
 * https://oeis.org/A023991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 426, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A023991 – Sum of exponents of primes in the multinomial coefficient
 *
 *  a(n) = Σp [ v_p((3n)!) – v_p((n+1)!) – v_p(n!) – v_p((n–1)!) ]
 *  where v_p(m!) is the exponent of prime p in the factorisation of m!.
 *  Compute it by Legendre’s formula and a simple primes sieve.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache for primes generated up to prime_limit */
static uint64_t *primes = NULL;   /* array of primes up to prime_limit   */
static size_t    prime_cnt = 0;   /* number of primes in the array      */
static idx_t     prime_limit = 0; /* largest number up to which primes */
                                   /* have been generated (3·n)          */

/* ------------------------------------------------------------------ */
/* ensure_primes: generate all primes ≤ limit (simple sieve).         */
/* The routine is called lazily, increasing the cache only when      */
/* needed.  The limit is typically 3·n, small enough for a 64‑bit     */
/* array of bools.                                                    */
/* ------------------------------------------------------------------ */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return; /* already enough */

    /* Allocate a temporary boolean sieve */
    bool *is_composite = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_composite)
        exit(1); /* out of memory */

    for (idx_t i = 0; i <= limit; ++i)
        is_composite[i] = false;

    /* Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes and rebuild the cache */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++cnt;

    uint64_t *new_primes = (uint64_t *)realloc(primes, cnt * sizeof(uint64_t));
    if (!new_primes)
        exit(1);
    primes = new_primes;
    prime_cnt = cnt;
    prime_limit = limit;

    /* Store primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            primes[pos++] = (uint64_t)i;

    free(is_composite);
}

/* ------------------------------------------------------------------ */
/* Legendre's formula: exponent of prime p in m!                     */
/* ------------------------------------------------------------------ */
static seq_t v_p_factorial(idx_t m, idx_t p)
{
    seq_t res = 0;
    if (p > m)
        return 0;
    idx_t power = p;
    while (power <= m) {
        res += (seq_t)(m / power);
        if (power > m / p)
            break;            /* avoid overflow of power *= p */
        power *= p;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for OEIS offset but safe to return 0 */
        return (seq_t)0;

    /* Ensure primes up to 3·n are available */
    if ((3 * n) > prime_limit)
        ensure_primes(3 * n);

    seq_t sum = 0;

    idx_t m1 = 3 * n;
    idx_t m2 = n + 1;
    idx_t m3 = n;
    idx_t m4 = (n > 0) ? n - 1 : 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > m1)
            break;
        seq_t v = 0;
        v += v_p_factorial(m1, p);
        v -= v_p_factorial(m2, p);
        v -= v_p_factorial(m3, p);
        v -= v_p_factorial(m4, p);
        sum += v;
    }

    return sum;
}
