/*
 * A099181  Primes p which are greater than twice p reversed.
 * https://oeis.org/A099181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   append_seq_t, append_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth prime p such that p > 2 * rev(p),
   where rev(p) is the decimal reversal of p.  The
   function lazily generates the required primes and
   stores the qualifying ones.  It works for any order
   of calls and for all n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_seq_t.h"
#include "../utils/append_uint64.h"

/* Test primality of x using the list of primes that is
   dynamically maintained.  The list contains all primes
   found so far (including those larger than sqrt(x)). */
static bool is_prime(uint64_t x, uint64_t *prime_list, size_t primes_len)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i)
    {
        uint64_t q = prime_list[i];
        if ((unsigned __int128)q * q > x) break;
        if (x % q == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 */

    /* Static storage for generated terms */
    static seq_t  *terms      = NULL;
    static size_t terms_cap  = 0;
    static size_t terms_len  = 0;

    /* Static storage for prime generation */
    static uint64_t *prime_list   = NULL;
    static size_t    primes_cap   = 0;
    static size_t    primes_len   = 0;
    static uint64_t  candidate   = 3;   /* next odd candidate to test */
    static bool     first_run   = true;

    /* First run: initialise data structures */
    if (first_run)
    {
        /* Initialise prime list with the first prime 2 */
        primes_cap = 16;
        prime_list = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        primes_len = 1;

        /* Initialise terms array */
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;

        /* Check the prime 2 itself */
        uint64_t rev = 0, tmp = 2;
        while (tmp) { rev = rev * 10 + tmp % 10; tmp /= 10; }
        if (2 > 2 * rev) append_seq_t(&terms, &terms_cap, &terms_len, (seq_t)2);

        first_run = false;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < n)
    {
        /* Find the next prime */
        uint64_t p;
        while (true)
        {
            if (is_prime(candidate, prime_list, primes_len))
            {
                p = candidate;
                /* Store this prime for future primality tests */
                append_uint64(&prime_list, &primes_cap, &primes_len, p);
                candidate += 2;          /* next odd candidate */
                break;
            }
            candidate += 2;
        }

        /* Evaluate condition p > 2 * rev(p) */
        uint64_t rev = 0, tmp = p;
        while (tmp) { rev = rev * 10 + tmp % 10; tmp /= 10; }
        if (p > 2 * rev)
            append_seq_t(&terms, &terms_cap, &terms_len, (seq_t)p);
    }

    return terms[n - 1];
}
