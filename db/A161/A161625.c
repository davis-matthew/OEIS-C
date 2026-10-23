/*
 * A161625  Sum of all numbers from 1 up to the final digit of prime(n).
 * https://oeis.org/A161625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = A000217(A007652(n)) // triangular number of the final
 *            digit of the n‑th prime
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* static prime cache ----------------------------------------------------- */
static idx_t *prime_list   = NULL;   /* array of primes in increasing order */
static size_t   prime_count = 0;     /* number of primes stored in prime_list */
static idx_t    sieve_limit = 0;     /* largest number that has been fully sieved */

/* Performs a classical Eratosthenes sieve up to 'limit' and stores all primes */
static void sieve_up_to(idx_t limit)
{
    /* allocate and initialise the primality bitmap */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime) return;  /* allocation failure – leave cache unchanged */

    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    idx_t sqrt_lim = (idx_t)sqrt((double)limit);
    for (size_t i = 2; i <= (size_t)sqrt_lim; ++i)
        if (is_prime[i])
            for (size_t j = (size_t)i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;

    /* count primes to size the storage array */
    size_t new_prime_count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++new_prime_count;

    /* allocate storage for the primes */
    idx_t *new_prime_list = (idx_t *)malloc(new_prime_count * sizeof(idx_t));
    if (!new_prime_list) {
        free(is_prime);
        return; /* allocation failure – leave cache unchanged */
    }

    /* copy primes into the new array */
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) new_prime_list[idx++] = (idx_t)i;

    free(is_prime);

    /* replace the old cache */
    free(prime_list);
    prime_list   = new_prime_list;
    prime_count  = new_prime_count;
    sieve_limit  = limit;
}

/* Ensures that at least 'n' primes are stored in prime_list */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;  /* enough primes cached */

    /* Rough bound for the n-th prime: p_n < n (log n + log log n) for n >= 6 */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double nn = (double)n;
        limit = (idx_t)(nn * (log(nn) + log(log(nn))) + 10.0); /* safety margin */
    }

    if (limit <= sieve_limit) return; /* already sieved far enough */

    sieve_up_to(limit);
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but return 0 */

    ensure_primes(n);

    /* nth prime is stored at prime_list[n-1] (1‑based indexing) */
    idx_t last_digit = prime_list[n - 1] % 10;

    /* triangular number T(k) = k*(k+1)/2, computed in seq_t domain */
    seq_t k = (seq_t)last_digit;
    seq_t result = k * (k + (seq_t)1) / (seq_t)2;
    return result;
}
