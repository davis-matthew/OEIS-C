/*
 * A085655  Difference between number of primes not exceeding reversed n and
 *          n.
 * https://oeis.org/A085655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = pi(rev(n)) - pi(n)
 * Range:     i8 n <= 799, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Includes ---------- */
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Static data for the sieve ---------- */
static idx_t sieve_limit = 0;      /* highest number currently sieved   */
static bool  *is_prime   = NULL;   /* primality flags (true = prime)   */
static seq_t *pi_table   = NULL;   /* prefix counts of primes up to i  */

/* ---------- Reverse the decimal digits of x ---------- */
static idx_t rev_digits(idx_t x)
{
    idx_t rev = 0;
    while (x > 0)
    {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev;
}

/* ---------- Ensure the sieve covers all numbers up to n ---------- */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_limit)
        return;                     /* already covered */

    /* Grow the sieve to at least n (and a small buffer to avoid frequent reallocs) */
    idx_t new_limit = (n > 16) ? n : 16;

    /* Allocate new arrays */
    bool  *new_is_prime = (bool *)malloc((new_limit + 1) * sizeof(bool));
    seq_t *new_pi       = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
    if (!new_is_prime || !new_pi)
    {
        /* Allocation failure – aborting is acceptable for this challenge */
        exit(EXIT_FAILURE);
    }

    /* Assume all numbers are prime initially */
    for (idx_t i = 0; i <= new_limit; ++i)
        new_is_prime[i] = true;

    /* 0 and 1 are not prime */
    if (new_limit >= 0) new_is_prime[0] = false;
    if (new_limit >= 1) new_is_prime[1] = false;

    /* Mark composites using the classic Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= new_limit; ++p)
    {
        if (new_is_prime[p])
        {
            for (idx_t j = p * p; j <= new_limit; j += p)
                new_is_prime[j] = false;
        }
    }

    /* Build the prefix prime‑count table */
    seq_t count = 0;
    for (idx_t i = 0; i <= new_limit; ++i)
    {
        if (new_is_prime[i])
            ++count;
        new_pi[i] = count;
    }

    /* Replace old tables */
    free(is_prime);
    free(pi_table);
    is_prime = new_is_prime;
    pi_table = new_pi;
    sieve_limit = new_limit;
}

/* ---------- Prime counting function using the sieve ---------- */
static seq_t prime_pi(idx_t n)
{
    if (n > sieve_limit)
        ensure_sieve(n);
    return pi_table[n];
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1, but safe guard */

    idx_t rev_n = rev_digits(n);
    seq_t pi_n   = prime_pi(n);
    seq_t pi_rev = prime_pi(rev_n);
    return pi_rev - pi_n;
}
