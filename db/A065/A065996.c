/*
 * A065996  a(n) = prime(prime(n) mod n).
 * https://oeis.org/A065996
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n) mod n) (offset 2)
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Store primes found so far (0‑based index: prime_list[0]=2). */
static idx_t *prime_list = NULL;   /* dynamic array of primes */
static size_t prime_list_len = 0;  /* number of primes stored */

/* Ensure that prime_list contains at least n primes. */
static void ensure_primes(size_t n_needed)
{
    if (prime_list_len >= n_needed)
        return;

    /* Upper bound for the nth prime (Rosser's theorem). */
    double nd = (double)n_needed;
    idx_t limit;
    if (n_needed < 6)
        limit = 15;
    else
        limit = (idx_t)(nd * (log(nd) + log(log(nd))) + 10.0);

    /* Simple sieve of Eratosthenes up to 'limit'. */
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    for (idx_t p = 2; p * p <= limit; ++p)
        if (!sieve[p])
            for (idx_t j = p * p; j <= limit; j += p)
                sieve[j] = true;

    /* Count how many primes we will have. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            ++count;

    /* Resize the primes array. */
    prime_list = (idx_t *)realloc(prime_list, count * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            prime_list[pos++] = i;

    prime_list_len = count;
    free(sieve);
}

/* Exported sequence generator.  n is assumed to be >= 2.  */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* behaviour below the offset is irrelevant */

    ensure_primes((size_t)n);          /* need the n‑th prime */
    idx_t p_n = prime_list[n - 1];      /* prime(n)  (0‑based array) */

    idx_t r = p_n % n;                  /* r in 1 … n‑1 for n ≥ 2 */
    if (r == 0)                         /* safety: should not happen */
        r = n;

    ensure_primes((size_t)r);          /* need the r‑th prime */
    seq_t result = (seq_t)prime_list[r - 1];
    return result;
}
