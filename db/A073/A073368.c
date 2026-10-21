/*
 * A073368  Remainder when n is divided by number of composites not exceeding
 *          n.
 * https://oeis.org/A073368
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Mod[n, n - Pi(n) - 1] (for n ≥ 4)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool *prime_arr = NULL;   /* true if index is prime, otherwise false */
static idx_t *pi_arr   = NULL;   /* pi_arr[i] = number of primes ≤ i */
static idx_t max_n = 0;          /* maximum index for which the sieve has been built */

/* Rebuild a sieve up to at least 'n' and compute pi_arr up to that value. */
static void ensure_sieve(idx_t n)
{
    if (n <= max_n)                /* already have sufficient data */
        return;

    /* Allocate storage for indices 0 .. n */
    size_t size = (size_t)n + 1;
    bool *new_prime_arr = (bool *)realloc(prime_arr, size * sizeof(bool));
    idx_t *new_pi_arr   = (idx_t *)realloc(pi_arr,   size * sizeof(idx_t));
    if (!new_prime_arr || !new_pi_arr) {
        /* Allocation failed – abort the program (could be handled more gracefully) */
        abort();
    }
    prime_arr = new_prime_arr;
    pi_arr    = new_pi_arr;

    /* Initialize all entries as potentially prime */
    for (size_t i = 0; i < size; ++i)
        prime_arr[i] = true;

    /* 0 and 1 are not prime */
    if (size > 0) prime_arr[0] = false;
    if (size > 1) prime_arr[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (prime_arr[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                prime_arr[j] = false;
        }
    }

    /* Compute prefix sums of primes (pi function) */
    idx_t cumulative = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (prime_arr[i])
            ++cumulative;
        pi_arr[i] = cumulative;
    }

    max_n = n;
}

/* Return a(n) = n mod (n - pi(n) - 1) for n ≥ 4 (behavior for n < 4 is irrelevant). */
seq_t a(idx_t n)
{
    ensure_sieve(n);

    idx_t pi_n   = pi_arr[n];           /* number of primes ≤ n */
    seq_t sn     = (seq_t)n;            /* n in seq_t type */
    seq_t divisor = sn - (seq_t)pi_n - 1; /* number of composites ≤ n */
    seq_t result = sn % divisor;        /* remainder when n is divided by composites count */
    return result;
}
