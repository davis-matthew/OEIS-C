/*
 * A055377  a(n) = largest prime <= n/2.
 * https://oeis.org/A055377
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime ≤ n/2 (OEIS A055377)
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve up to the required limit each time it is needed. */

#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve state */
static bool *prime_flag   = NULL;   /* prime_flag[i] is true iff i is prime */
static idx_t *prime_list = NULL;    /* sorted list of all primes found */
static size_t primes_cnt = 0;       /* number of primes in prime_list          */
static idx_t  limit_used = 0;       /* sieve is valid up to indices ≤ limit_used */

/* Rebuild the prime sieve up to at least 'limit' (inclusive) */
static void
ensure_primes(idx_t limit)
{
    if (limit <= limit_used) return;  /* already up to date */

    /* Allocate/resize the prime_flag array */
    bool *tmp = (bool *)realloc(prime_flag, (size_t)(limit + 1));
    if (!tmp) exit(EXIT_FAILURE);     /* memory allocation failure */
    prime_flag = tmp;

    /* Initialise all entries as prime, except 0 and 1 */
    for (idx_t i = 0; i <= limit; ++i)
        prime_flag[i] = (i >= 2);

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = false;
        }
    }

    /* Count primes for the new list */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) ++cnt;

    idx_t *tmp_list = (idx_t *)realloc(prime_list, (size_t)cnt * sizeof(idx_t));
    if (!tmp_list) exit(EXIT_FAILURE);
    prime_list = tmp_list;
    primes_cnt = (size_t)cnt;

    /* Fill the list */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) prime_list[pos++] = i;

    limit_used = limit;
}

/* Return the largest prime ≤ x (x ≥ 2) */
static idx_t
largest_prime_leq(idx_t x)
{
    if (x < 2) return 0; /* safety for n < 4 */

    ensure_primes(x);    /* ensure prime data up to x */

    /* Binary search on the primes list */
    size_t lo = 0, hi = primes_cnt ? primes_cnt - 1 : 0;
    idx_t best = 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        idx_t p = prime_list[mid];
        if (p <= x) {
            best = p;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;           /* undefined for n < 4, but harmless */
    idx_t half = n / 2;
    idx_t p    = largest_prime_leq(half);
    return (seq_t)p;
}
