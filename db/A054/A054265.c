/*
 * A054265  Sum of composite numbers between successive primes.
 * https://oeis.org/A054265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 428, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (prime(n+1) + prime(n)) * (prime(n+1) - prime(n) - 1) / 2
 * (OEIS A054265 – sum of composite numbers between successive primes)
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*  seq_t and idx_t are typedef'd by the build system */
static seq_t *prime_arr    = NULL; /* 1-indexed array of primes */
static size_t prime_count  = 0;     /* number of primes stored */

/* Ensure that primes up to index `needed` (inclusive) are generated.
   For n ≥ 6, p_n < n (log n + log log n).  A small safety margin is added.
   A simple sieve of Eratosthenes is used.  This routine may be called
   many times; it only recomputes when more primes are required. */
static void
ensure_primes(idx_t needed)
{
    if (prime_count >= needed + 1)      /* already have enough primes */
        return;

    /* Estimate upper bound for the required prime */
    double d = (double)needed;          /* needed is at least 0 */
    double upper;
    if (d < 6.0)
        upper = 15.0;
    else
        upper = d * (log(d) + log(log(d))) + 3.0;   /* safety margin */

    size_t limit = (size_t)upper + 1;   /* inclusive */

    /* Simple sieve */
    char *is_prime = (char *)malloc(limit + 1);
    if (!is_prime) abort();

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }
    }

    /* Count primes to resize array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate / reallocate primes array (1‑indexed) */
    seq_t *new_arr = (seq_t *)malloc((cnt + 1) * sizeof(seq_t));
    if (!new_arr) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            ++idx;
            new_arr[idx] = (seq_t)i;     /* cast to seq_t */
        }
    }

    free(is_prime);

    if (prime_arr)
        free(prime_arr);

    prime_arr   = new_arr;
    prime_count = idx;
}

/*  Compute a(n) for n ≥ 1.  Behaviour for n < 1 is irrelevant. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;        /* offset: a(1) is the first term */

    ensure_primes(n + 1);            /* need primes p_n and p_{n+1} */

    seq_t p_n   = prime_arr[n];     /* prime(n)   */
    seq_t p_np1 = prime_arr[n + 1]; /* prime(n+1) */

    seq_t diff = p_np1 - p_n - (seq_t)1;       /* p_{n+1} - p_n - 1 */
    seq_t sum  = (p_n + p_np1) * diff / (seq_t)2; /* formula */

    return sum;
}
