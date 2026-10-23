/*
 * A023822  Sum of exponents in prime-power factorization of C(3n,n-3).
 * https://oeis.org/A023822
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p <= 3n} ( v_p((3n)!) - v_p((n-3)!) - v_p((2n+3)!) )
 *            where v_p(m!) is the exponent of the prime p in the
 *            prime‑factorization of m! (Legendre's formula). The function
 *            works for all n ≥ 3 and returns 0 for n < 3. It caches the
 *            primes up to the largest 3n encountered so subsequent calls...
 * Range:     u8 n <= 859, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include minimal headers needed for allocation and boolean type */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static state for a cached list of primes */
static uint32_t *primes = NULL;      /* array of primes up to current limit */
static size_t   primes_cnt = 0;      /* number of primes stored */
static idx_t    primes_max = 0;      /* current upper bound 3n for primes */

/* Build or extend the prime list up to a given limit (inclusive).  This
   function is thread‑safe only for single‑threaded evaluation, which
   is the expected use in the OEIS environment. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_max) return; /* already sufficient */

    /* Free previous prime array if any */
    if (primes != NULL) {
        free(primes);
        primes = NULL;
    }

    /* Sieve of Eratosthenes up to 'limit' */
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_prime) return;          /* memory failure – we will return 0 later */

    /* Mark all as prime initially, except 0 and 1 */
    for (size_t i = 2; i <= (size_t)limit; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate and fill prime array */
    uint32_t *p_arr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!p_arr) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) p_arr[idx++] = (uint32_t)i;

    free(is_prime);

    primes      = p_arr;
    primes_cnt  = cnt;
    primes_max  = limit;
}

/* Return sum of exponents of all prime powers in binomial(3n, n‑3). */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3. */
    if (n < 3) return 0;

    /* Ensure we have all primes up to 3n */
    idx_t max_prime_needed = 3 * n;
    ensure_primes(max_prime_needed);

    /* If prime list could not be built, return 0 */
    if (primes == NULL) return 0;

    /* Compute binomial exponent sum using Legendre's formula */
    seq_t sum = 0;
    idx_t N  = 3 * n;     /* (3n)! */
    idx_t K1 = n - 3;     /* (n-3)!  */
    idx_t K2 = 2 * n + 3; /* (2n+3)!*/

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if (p > N) break; /* no larger primes contribute */

        seq_t e = 0;          /* exponent of p in the binomial */

        /* v_p((3n)!) */
        idx_t t = N;
        while (t) { t /= p; e += t; }

        /* subtract v_p((n-3)!) */
        t = K1;
        while (t) { t /= p; e -= t; }

        /* subtract v_p((2n+3)!) */
        t = K2;
        while (t) { t /= p; e -= t; }

        sum += e;
    }

    return sum;
}
