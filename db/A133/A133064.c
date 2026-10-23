/*
 * A133064  a(n) = 5*p^5 + 3*p^3 + 2*p^2, where p = prime(n).
 * https://oeis.org/A133064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*prime(n)^5 + 3*prime(n)^3 + 2*prime(n)^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Simple dynamic sieve: generate as many primes as needed,
   storing them in a static array that grows on demand. */
static uint64_t *prime_list = NULL;     /* array of primes */
static size_t    prime_cnt  = 0;        /* number of primes stored */
static size_t    prime_cap  = 0;        /* allocated capacity */
static uint64_t  candidate  = 2;        /* current candidate to test */

/* Push a new prime into the static array, expanding if necessary. */
static void push_prime(uint64_t p)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                       /* unrecoverable allocation failure */
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Test whether x is prime using the already known primes. */
static int is_prime_candidate(uint64_t x)
{
    uint64_t limit = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Ensure that at least n primes are present in prime_list. */
static void ensure_primes(idx_t n)
{
    while (prime_cnt < n) {
        while (!is_prime_candidate(candidate))
            ++candidate;
        push_prime(candidate);
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; n < 1 is undefined – return 0. */
    if (n < 1) return 0;

    ensure_primes(n);

    seq_t p  = (seq_t)prime_list[n - 1];
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p5 = p3 * p2;              /* p^5 */

    return 5 * p5 + 3 * p3 + 2 * p2;
}
