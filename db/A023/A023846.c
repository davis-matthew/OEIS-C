/*
 * A023846  Sum of exponents in prime-power factorization of binomial(5n,
 *          n+4).
 * https://oeis.org/A023846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_p ( vₚ((5n)!) – vₚ((n+4)!) – vₚ((4n-4)!) ), i.e. the
 *            sum of exponents in the prime‑power factorization of
 *            binomial(5n, n+4)
 * Range:     u8 n <= 665, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Prime generation
   ------------------------------------------------------------------ */
static idx_t *prime_list   = NULL;  /* array of primes */
static size_t prime_count  = 0;     /* number of primes stored */
static idx_t  prime_limit  = 0;     /* largest n for which primes are valid */

/* Rebuild prime_list to contain all primes ≤ limit.  This function
   is called only when a larger limit is requested. */
static void rebuild_primes(idx_t limit)
{
    if (limit < 2) {
        /* No primes needed */
        free(prime_list);
        prime_list  = NULL;
        prime_count = 0;
        prime_limit = 0;
        return;
    }

    /* Allocate a bitmap of size limit+1 (uint8_t is fine). */
    size_t sz = (size_t)limit + 1ULL;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) {
        /* Allocation failed – leave global state unchanged. */
        return;
    }
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Simple sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = 0;
        }
    }

    /* Count primes to allocate array */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++count;

    /* Allocate and fill prime array */
    idx_t *primes = (idx_t *)malloc(count * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            primes[idx++] = i;

    /* Free old data and update globals */
    free(prime_list);
    free(is_prime);
    prime_list  = primes;
    prime_count = count;
    prime_limit = limit;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is first element.  For n<1 the behaviour is
       irrelevant, but returning 0 keeps the function well‑defined. */
    if (n < 1) return (seq_t)0;

    /* Ensure we have primes up to 5·n.  This may cause a rebuild. */
    idx_t needed_limit = 5 * n;
    if (needed_limit > prime_limit)
        rebuild_primes(needed_limit);

    /* Parameters for Legendre’s formula */
    idx_t N5 = 5 * n;      /* (5n)! */
    idx_t N1 = n + 4;      /* (n+4)! */
    idx_t N4 = 4 * n - 4;  /* (4n-4)! */

    seq_t total = (seq_t)0;

    /* Sum the exponent contributions of each prime. */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > N5) break;

        idx_t exp = 0;
        idx_t power = p;
        while (power <= N5) {
            exp += N5 / power - N1 / power - N4 / power;
            if (power > N5 / p) break; /* prevent overflow of power */
            power *= p;
        }
        total += (seq_t)exp;
    }

    return total;
}
