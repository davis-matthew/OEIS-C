/*
 * A094742  Duplicate of A008578.
 * https://oeis.org/A094742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 1; for n > 1, a(n) = (n‑1)-th prime number
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime generation with a simple sieve.  */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Types supplied by the build.  */
extern int64_t seq_nth_prime(idx_t n); /* just to silence unused variable. */

/* Static storage for the list of primes.  */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;   /* number of primes stored */
static size_t prime_limit = 0;   /* sieve limit used for current list */

/* Compute an upper bound for the nth prime.
   For n >= 6, the bound n*(log n + log log n) is safe.  */
static size_t prime_upper_bound(size_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double bound = dn * (log(dn) + log(log(dn))) + 3.0;
    return (size_t)bound;
}

/* Ensure that prime_list contains at least need primes. */
static void ensure_primes(size_t need)
{
    if (prime_count >= need) return;   /* already enough */

    /* Allocate a new sieve up to a sufficient bound.  */
    size_t limit = prime_upper_bound(need);
    unsigned char *is_prime = (unsigned char*)malloc(limit + 1);
    if (!is_prime) { exit(1); }

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    size_t sqrt_limit = (size_t)sqrt((double)limit);
    for (size_t p = 2; p <= sqrt_limit; ++p) {
        if (is_prime[p]) {
            for (size_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = 0;
        }
    }

    /* Count how many primes we found.  */
    size_t new_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_count++;

    /* Allocate storage for the new list.  */
    uint32_t *new_list = (uint32_t*)realloc(prime_list, new_count * sizeof(uint32_t));
    if (!new_list) { free(is_prime); exit(1); }
    prime_list = new_list;
    prime_count = new_count;
    prime_limit = limit;

    /* Fill the list.  */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    free(is_prime);
}

/* The required OEIS sequence.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    /* We need the (n-1)-th prime.  */
    size_t need = (size_t)(n - 1);
    ensure_primes(need);
    return (seq_t)prime_list[need - 1];
}
