/*
 * A085975  Number of 1's in decimal expansion of prime(n).
 * https://oeis.org/A085975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digit 1's in the n-th prime number
 *            (i.e. a(1)=0, a(2)=0, a(5)=2, etc.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/count_ones.h"

/* ---------- internal state ----------------------------------------- */
static idx_t *prime_list = NULL;   /* array of found primes */
static size_t prime_count = 0;      /* number of primes stored   */
static size_t prime_cap   = 0;      /* current allocated size    */

/* ---------- utility functions --------------------------------------- */
/* Grow the prime array if needed. */
static void grow_prime_array(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 128;
    idx_t *new_arr = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!new_arr) abort();          /* unrecoverable allocation failure */
    prime_list = new_arr;
    prime_cap   = new_cap;
}

/* Test whether x is prime using the primes already generated. */
static int is_prime_candidate(idx_t x)
{
    if (x < 2) return 0;
    if (x == 2 || x == 3) return 1;
    if ((x & 1) == 0) return 0;    /* even numbers >2 are not prime */

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > x / p) break;      /* p*p > x → no more divisors */
        if (x % p == 0) return 0;  /* divisible by a found prime */
    }
    return 1;
}

/* Ensure that the first n primes are in prime_list. */
static void ensure_n_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;   /* already have enough */

    /* Initialise storage if this is the first call. */
    if (prime_cap == 0) {
        prime_cap = 128;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        if (!prime_list) abort();
    }

    /* Fill the prime table up to the n-th prime. */
    if (prime_count == 0) {
        /* First prime is 2. */
        prime_list[prime_count++] = 2;
    }

    /* Candidate generation: start after the last stored prime.
       After 2, we only need to check odd numbers. */
    idx_t candidate;
    if (prime_count == 1)
        candidate = 3;                 /* after 2 */
    else
        candidate = prime_list[prime_count-1] + 2;

    while (prime_count < (size_t)n) {
        if (is_prime_candidate(candidate)) {
            if (prime_count >= prime_cap)
                grow_prime_array();
            prime_list[prime_count++] = candidate;
        }
        candidate += 2;                /* skip even numbers */
    }
}

/* ---------- public sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, return 0 */

    ensure_n_primes(n);
    /* primes are stored 0‑based: prime_list[0] is the 1st prime */
    return count_ones(prime_list[n-1]);
}
