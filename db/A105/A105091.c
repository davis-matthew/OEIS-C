/*
 * A105091  Sum of the right diagonal in ordered 3 X 3 prime squares.
 * https://oeis.org/A105091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000040(9*n-6) + A000040(9*n-4) + A000040(9*n-2)
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sieve primes up to a safe upper bound and sum the three
   required primes.  The sieve is performed on first call and rebuilt
   only if a larger n is requested.  The implementation is fully
   compatible with both C11 and C++17. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static seq_t *primes = NULL;        /* array of primes */
static idx_t prime_count = 0;       /* number of primes stored */
static size_t sieve_limit = 0;      /* current sieve upper bound */

/* Perform a simple sieve up to 'limit' and store primes in 'primes' */
static void build_primes(size_t limit)
{
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;

    /* Count primes to allocate array */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    /* Allocate or reallocate primes array */
    if (primes) free(primes);
    primes = (seq_t*)malloc(count * sizeof(seq_t));
    if (!primes) exit(1);

    /* Fill primes array */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (seq_t)i;

    prime_count = (idx_t)count;
    sieve_limit = limit;
    free(is_prime);
}

/* Ensure we have at least 'needed' primes available */
static void ensure_primes(idx_t needed)
{
    if ((idx_t)prime_count >= needed) return;
    /* compute upper bound for the needed-th prime using the
       Rosser–Schoenfeld estimate */
    double n = (double)needed;
    size_t limit;
    if (needed < 6) limit = 15;
    else
        limit = (size_t)(n * (log(n) + log(log(n)))) + 3;
    /* Keep a modest lower bound to avoid tiny re-sieves */
    if (limit < 2000000) limit = 2000000;
    if (limit > sieve_limit) build_primes(limit);
}

/* Main function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset <1, return 0 */
    /* We need primes up to index 9*n */
    ensure_primes(9 * n);
    /* Positions in the prime list are 1‑based.  Convert to 0‑based: */
    size_t pos = (size_t)(9 * n);
    size_t i1 = pos - 7;   /* 9*n - 6  → index 9*n-7 */
    size_t i2 = pos - 5;   /* 9*n - 4  → index 9*n-5 */
    size_t i3 = pos - 3;   /* 9*n - 2  → index 9*n-3 */
    seq_t sum = primes[i1] + primes[i2] + primes[i3];
    return sum;
}
