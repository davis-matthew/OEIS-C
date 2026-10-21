/*
 * A213631  Primes p such that primality of (p+p')/2+4 and of (p+p')/2-4
 *          differ, where p'=nextprime(p+1), the next larger prime.
 * https://oeis.org/A213631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: The sequence lists primes p such that the primalities
 * of (p + nextprime(p+1)) / 2 + 4 and (p + nextprime(p+1)) / 2 – 4
 * differ.  That is, exactly one of those two numbers is prime.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Build a list of primes up to a given limit */
#include "../utils/build_primes_upto.h"
/* Return the next prime greater than start */
#include "../utils/next_prime.h"
/* Test whether an unsigned integer <= 64‑bit is prime */
#include "../utils/is_prime.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static uint64_t prime_lim = 0;

/* Ensure we have primes up to at least n (inclusive) */
static void ensure_primes(uint64_t n)
{
    if (n <= prime_lim)
        return;

    /* Expand limit (doubling strategy) */
    uint64_t new_lim = prime_lim ? prime_lim * 2 : 100;
    while (new_lim < n)
        new_lim *= 2;

    /* Rebuild list */
    if (prime_list)
        free(prime_list);
    build_primes_upto(new_lim, &prime_list, &prime_cnt);
    prime_lim = new_lim;
}

/* Find the next prime after p (exclusive) */
static uint64_t next_prime_after(uint64_t p)
{
    uint64_t idx = 0;
    /* matrix: if p is inside current list and not the last element,
       the next prime is prime_list[idx+1].  Otherwise call the
       utility, which is safe even if the prime is beyond the list. */
    if (idx < prime_cnt) {
        while (idx < prime_cnt && prime_list[idx] <= p)
            ++idx;
        if (idx < prime_cnt)
            return prime_list[idx];
    }
    return next_prime(p + 1);
}

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but return 0 */
        return (seq_t)0;

    size_t count = 0;          /* how many qualifying primes found */
    size_t i = 0;              /* index into prime_list */

    /* Start with a modest limit; will grow if needed */
    ensure_primes(100);

    while (1) {
        /* Ensure next prime exists in our table; if not, grow the list */
        if (i + 1 == prime_cnt) {
            /* The next prime after the current one might lie beyond
               current limit; enlarge buffer and rebuild. */
            ensure_primes(prime_lim * 2);
        }

        uint64_t p = prime_list[i++];
        uint64_t p_next = next_prime_after(p);

        /* A(5) is the small prime 5.  For p slightly above the
           limit, next_prime_after will recurse beyond the table. */
        uint64_t m = (p + p_next) / 2;   /* integer division -- guaranteed integer */

        bool prime_plus  = is_prime(m + 4);
        bool prime_minus = is_prime(m - 4);

        if (prime_plus ^ prime_minus) {  /* exactly one prime */
            ++count;
            if (count == (size_t)n)
                return (seq_t)p;
        }

        /* If we've gone beyond our current list and haven't found
           enough terms yet, double the limit. */
        if (i >= prime_cnt) {
            ensure_primes(prime_lim * 2);
        }
    }
}
