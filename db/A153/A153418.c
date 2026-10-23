/*
 * A153418  Primes p such that p+18 is also prime.
 * https://oeis.org/A153418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* */
/*  Sequence A153418 : primes p such that p+18 is also prime.
 *  a(1)=5, a(2)=11, a(3)=13, a(4)=19, a(5)=23, ...
 *  The implementation builds a list of primes up to an incrementally
 *  increasing bound and counts those whose successor separated by 18
 *  is also prime.  Memoisation of the prime table allows arbitrary
 *  access order of n.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Memoised prime data ---------------------------------------------- */

static idx_t   current_limit = 0;   /* upper bound used in current list   */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;

/* Helper: ensure prime list covers up to `limit` -------------------- */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= current_limit) /* already sufficient */ return;
    if (prime_list) free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    current_limit = limit;
}

/* Main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset behaviour does not matter */

    /* initial bound – small enough to get a few terms quickly         */
    if (current_limit == 0) {
        current_limit = 2000;        /* ~ 300 primes – more than 25 terms */
        build_primes_upto(current_limit, &prime_list, &prime_count);
    }

    while (1) {
        idx_t count = 0;            /* number of qualifying primes found */

        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];

            /* Skip primes that are too large to have p+18 within bound
               – the primality test works for any value. */
            if (p + 18 > 0xffffffffUL && /* harmless guard for overflow */
                !is_prime((idx_t)(p + 18))) continue;

            if (is_prime((idx_t)(p + 18))) {
                ++count;
                if (count == n) {
                    /* cast to seq_t (guaranteed to fit for n≤10 000) */
                    return (seq_t)p;
                }
            }
        }

        /* not enough found: double the limit and retry            */
        current_limit <<= 1;
        if (current_limit == 0) current_limit = 1ULL << 30; /* overflow catch */
        free(prime_list);  prime_list = NULL;  prime_count = 0;
        build_primes_upto(current_limit, &prime_list, &prime_count);
    }
}
