/*
 * A073797  a(n) = 2^n mod pi(n).
 * https://oeis.org/A073797
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n mod pi(n)
 * Range:     u8 n <= 1622, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple incremental sieve to compute
   pi(n) and binary exponentiation to evaluate 2^n modulo pi(n).
   It works for the generic integral `seq_t` type as required */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mod_pow_seq.h"

/* static data for the sieve */
static unsigned char *is_prime = NULL;   /* 1 if prime, 0 otherwise */
static int          *pi_prefix = NULL;   /* pi(i) = number of primes <= i */
static size_t          sieve_max = 0;     /* largest n already sieved */

/* Recompute the sieve up to n (inclusive).  Only called when n > sieve_max.
   As the requested values are small (n <= 10000 in tests) this simple
   full recomputation is inexpensive. */
static void sieve_up_to(size_t n)
{
    if (n <= sieve_max)
        return;                  /* already up to date */

    size_t new_size = n + 1;
    unsigned char *new_is_prime = (unsigned char *)realloc(is_prime, new_size);
    int           *new_pi_prefix = (int *)      realloc(pi_prefix, new_size * sizeof(int));

    is_prime  = new_is_prime;
    pi_prefix = new_pi_prefix;

    /* initialise all as potential primes */
    for (size_t i = 0; i < new_size; ++i)
        is_prime[i] = 1;

    if (new_size > 0) is_prime[0] = 0;   /* 0 is not prime */
    if (new_size > 1) is_prime[1] = 0;   /* 1 is not prime */

    /* classic Eratosthenes sieve */
    for (size_t p = 2; p * p <= n; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= n; m += p)
                is_prime[m] = 0;

    /* build the prefix sums for pi(i) */
    int count = 0;
    for (size_t i = 0; i <= n; ++i) {
        if (is_prime[i])
            ++count;
        pi_prefix[i] = count;
    }

    sieve_max = n;
}

/* Main sequence function: a(n) = 2^n mod pi(n)  (for n >= 2) */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* behaviour below offset is irrelevant */

    /* Ensure the sieve covers n */
    sieve_up_to((size_t)n);

    seq_t pi_val = (seq_t)pi_prefix[(size_t)n];   /* pi(n) */
    return mod_pow_seq((seq_t)2, (uint64_t)n, pi_val);
}
