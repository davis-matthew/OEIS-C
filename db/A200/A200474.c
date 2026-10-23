/*
 * A200474  a(n) = floor(10*(prime(n+1)-prime(n))/log(prime(n))^2).
 * https://oeis.org/A200474
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(10*(prime(n+1)-prime(n))/log(prime(n))^2), n >= 5
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Keep a simple global state for primes. */
static unsigned int *primes = NULL;          /* array of primes */
static size_t        prime_cnt = 0;         /* number of primes stored */
static size_t        sieve_limit = 0;       /* current sieve limit */

/* Generate at least `need` primes, storing them in `primes`. */
static void generate_primes(size_t need)
{
    if (prime_cnt >= need)
        return;                /* already have enough */

    /* Estimate an upper bound for the nth prime (Rosser's theorem). */
    double n = (double)need;
    double limit;
    if (need < 6)
        limit = 15.0;
    else
        limit = n * (log(n) + log(log(n))) + 3.0;   /* safety margin */

    size_t new_limit = (size_t)(limit) + 1;
    if (new_limit <= sieve_limit)
        new_limit = sieve_limit + 1;   /* make sure we grow */

    /* Sieve of Eratosthenes up to new_limit. */
    bool *is_prime = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!is_prime)
        return;   /* OOM – caller will fail gracefully */

    for (size_t i = 0; i <= new_limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Allocate new prime array to hold at least `need` primes. */
    unsigned int *new_primes = (unsigned int *)malloc(need * sizeof(unsigned int));
    if (!new_primes) {
        free(is_prime);
        return;   /* OOM */
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit && cnt < need; ++i) {
        if (is_prime[i]) {
            new_primes[cnt++] = (unsigned int)i;
        }
    }

    /* Replace old data. */
    free(primes);
    primes = new_primes;
    prime_cnt = cnt;
    sieve_limit = new_limit;

    free(is_prime);
}

/* Compute the nth term of OEIS A200474.  The sequence has offset 5,
 * but the function operates directly on the index n, where n >= 5. */
seq_t a(idx_t n)
{
    if (n < 5)
        return 0;   /* values below the offset are undefined */

    /* Ensure we have enough primes for prime(n) and prime(n+1). */
    generate_primes((size_t)n + 1);

    if (prime_cnt < (size_t)n + 1)
        return 0;   /* failed to generate primes */

    /* primes[0] = prime(1) */
    unsigned int p_n     = primes[(size_t)n - 1];
    unsigned int p_next  = primes[(size_t)n];
    unsigned int gap     = p_next - p_n;

    /* Compute the expression using double precision. */
    double ln = log((double)p_n);             /* natural logarithm */
    double denom = ln * ln;
    double value = 0.0;
    if (denom > 0.0)
        value = 10.0 * (double)gap / denom;

    /* Floor to the nearest integer, guarding against round‑off. */
    seq_t result = (seq_t)((value < 0.0) ? 0 : (int)(value + 1e-12));

    return result;
}
