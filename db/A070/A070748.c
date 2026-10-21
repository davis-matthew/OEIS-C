/*
 * A070748  a(n) = signum(sin(prime(n))), where signum=A057427, prime=A000040.
 * https://oeis.org/A070748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = signum( sin( prime(n) ) )
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  prime(n) is the n‑th prime, signum(x) = 1 if x>0, -1 if x<0, 0 if x==0 */

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool primes_built = false;   /* whether we already have a prime list   */
static idx_t *prime_list  = NULL;   /* array of primes, 0‑based indexing       */
static size_t primes_cnt  = 0;      /* number of primes stored                */

/*  Build a list of the first n primes using a simple sieve of Eratosthenes.  */
static void build_primes(idx_t n)
{
    if (primes_built && primes_cnt >= (size_t)n)
        return;                       /* already have enough primes            */

    /* Upper bound for the n-th prime:  n*(log n + log log n)  for n>=6   */
    uint64_t limit;
    if (n < 6)
        limit = 15;
    else
    {
        double dn  = (double)n;
        double l   = log(dn);
        double ll  = log(l);
        limit = (uint64_t)(dn * (l + ll)) + 3;   /* +3 safety margin          */
    }

    /* sieve up to 'limit' */
    bool *is_prime = (bool *)malloc((size_t)limit + 1U);
    if (!is_prime)
        return;                       /* allocation failure – caller will misbehave */

    for (size_t i = 0; i <= (size_t)limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= (size_t)limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;

    /* store the first n primes */
    if (prime_list)
        free(prime_list);
    prime_list = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!prime_list)
    {
        free(is_prime);
        return;
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit && cnt < (size_t)n; ++i)
        if (is_prime[i])
            prime_list[cnt++] = (idx_t)i;

    primes_cnt  = cnt;
    primes_built = true;
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined – sequence starts at offset 1 */

    build_primes(n);           /* ensure we have the n‑th prime          */
    idx_t p = prime_list[n - 1];  /* 0‑based index: prime_list[0] = 2      */

    long double s = sinl((long double)p);

    if (s > 0.0L)
        return (seq_t)1;
    if (s < 0.0L)
        return (seq_t)-1;
    /* sin(p) is never exactly zero for a positive integer p, but we keep
       the case for completeness.                                       */
    return (seq_t)0;
}
