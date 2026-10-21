/*
 * A092770  Cubes of A006450: a(n) = prime(prime(n))^3.
 * https://oeis.org/A092770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(n))^3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation builds a small prime sieve on demand
   using the upper bound given by Rosser's theorem.
   It caches the primes so that subsequent calls reuse the data.
   It works for any seq_t that supports the usual arithmetic operators
   (including built‑in integers and the big‑integer class used by the
   test harness). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* prime_cache holds the primes found so far, 0‑based index. */
static uint32_t *prime_cache = NULL;
static size_t    prime_cnt   = 0;   /* number of primes stored */

/* upper_bound_prime estimates an upper bound for the n-th prime.
   For n < 6 it returns 15, otherwise it uses the Rosser–Schoenfeld
   bound: n (log n + log log n). A small safety margin is added. */
static size_t upper_bound_prime(idx_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double ub = dn * (log(dn) + log(log(dn)));
    return (size_t)(ub) + 3;
}

/* ensure_primes_upto makes sure that prime_cache contains at least n primes.
   If more primes are required, a new sieve is built up to an
   estimated upper bound.  The sieve is always rebuilt from scratch
   (the cost is very small for the sizes we need). */
static void ensure_primes_upto(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;   /* already enough */

    size_t limit = upper_bound_prime(n);
    char *mark = (char*)calloc(limit + 1, 1);   /* 0 = prime candidate */
    if (!mark) exit(EXIT_FAILURE);

    for (size_t i = 2; i * i <= limit; ++i)
        if (!mark[i])
            for (size_t j = i * i; j <= limit; j += i)
                mark[j] = 1;

    /* Count primes to allocate exact storage */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!mark[i]) ++cnt;

    uint32_t *tmp = (uint32_t*)realloc(prime_cache, cnt * sizeof(uint32_t));
    if (!tmp && cnt > 0) exit(EXIT_FAILURE);
    prime_cache = tmp;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!mark[i]) prime_cache[idx++] = (uint32_t)i;

    prime_cnt = cnt;
    free(mark);
}

/* Returns the n-th prime (n ≥ 1).  The result is promoted to seq_t. */
static seq_t get_nth_prime(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    ensure_primes_upto(n);
    return (seq_t)prime_cache[n - 1];
}

/* Required OEIS sequence function. */
seq_t a(idx_t n)
{
    seq_t p1 = get_nth_prime(n);          /* prime(n)      */
    idx_t  idx2 = (idx_t)p1;              /* index for next prime  */
    seq_t p2 = get_nth_prime(idx2);       /* prime(prime(n)) */
    return p2 * p2 * p2;                  /* cube of that prime */
}
