/*
 * A048244  a(n) = A048106(A001405(n)).
 * https://oeis.org/A048244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 76, i32 n <= 176, i64 n <= 437,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A048244
 *
 *  a(n) = 2 * unitary(binom(n, ⌊n/2⌋)) - τ(binom(n, ⌊n/2⌋))
 *
 *  where τ(m) is the number of divisors of m and
 *  a unitary divisor d of m satisfies gcd(d, m/d) = 1.
 *  For any n ≥ 1 the result is returned as seq_t.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *primes = NULL;   /* list of primes up to the current limit */
static size_t   primes_cnt = 0;   /* number of primes stored */
static idx_t    primes_limit = 0; /* current maximum prime limit */

/*  Ensure that the global prime table contains all primes ≤ limit.
 *  The sieve is recomputed from scratch each time the limit increases.  */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;                     /* already sufficient */

    /* Boolean array for the sieve (0 = prime, 1 = composite). */
    bool *is_comp = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp)
        exit(1);

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
    }

    /* Rebuild the prime list.  */
    free(primes);
    primes = (uint64_t*)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!primes)
        exit(1);

    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            primes[primes_cnt++] = i;

    primes_limit = limit;
    free(is_comp);
}

/*  Main sequence function.  */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;          /* sequence is defined for n ≥ 1 */

    idx_t k = n / 2;      /* floor(n/2)  */
    ensure_primes(n);     /* need primes up to n */

    seq_t  total_div = 1; /* τ( binom(n,k) )  */
    size_t omega     = 0; /* distinct prime factors */

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p > n)
            break;

        /* Exponent of prime p in binom(n,k) = v_p(n!) - v_p(k!) - v_p((n-k)!) */
        idx_t e = 0;
        idx_t power = p;
        while (power <= n)
        {
            idx_t term = n / power - k / power - (n - k) / power;
            e += term;
            if (power > n / p)   /* avoid overflow */
                break;
            power *= p;
        }

        if (e)
        {
            omega++;
            total_div *= (seq_t)(e + 1);
        }
    }

    /* Number of unitary divisors = 2^omega.  */
    seq_t unitary = 1;
    for (size_t i = 0; i < omega; ++i)
        unitary *= (seq_t)2;

    /* a(n) = 2*unitary - total_div.  */
    return unitary * (seq_t)2 - total_div;
}
