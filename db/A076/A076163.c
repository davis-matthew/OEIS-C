/*
 * A076163  Primes p such that the absolute value of p minus the sum of
 *          squares of digits of p is also prime.
 * https://oeis.org/A076163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 648, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   digit_square_sum, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A076163 – primes p such that |p – sum of squares of digits of p| is prime.
 *
 *  The function keeps a static table of the terms found so far.  On each
 *  call it extends the table until it contains at least the requested
 *  index, performing a simple primality test and a digit‐sum test
 *  with the utilities is_prime_u64() and digit_square_sum().
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_u64.h"      /* bool is_prime_u64(uint64_t n); */
#include "../utils/digit_square_sum.h"  /* uint64_t digit_square_sum(uint64_t x); */

/* Return the n‑th term (a(1) is the first term).  For n==0 the function
 * returns 0; the call site should never use it. */
seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;

    /* -- Static state ---------------------------------------------------- */
    static bool initialized = false;
    static uint64_t *terms   = NULL;     /* array of qualifying primes (uint64_t) */
    static size_t terms_cnt  = 0;        /* number of stored terms            */
    static size_t terms_cap  = 0;        /* allocated capacity                 */
    static uint64_t candidate = 2;       /* next integer to test             */
    static size_t  prime_check_start = 2;/* index of next candidate for prime */

    if (!initialized) {
        initialized = true;
        terms_cap = 64;
        terms = (uint64_t *)malloc(terms_cap * sizeof(uint64_t));
        if (!terms) abort();
    }

    /* -- Generate terms until we have at least n of them ----------------- */
    while (terms_cnt < (size_t)n) {
        /* Find next prime */
        for (;;) {
            if (is_prime_u64(candidate)) break;
            ++candidate;
        }

        /* Compute |p - sum_of_squares_of_digits(p)| */
        uint64_t sq   = digit_square_sum(candidate);
        uint64_t diff = (candidate > sq) ? (candidate - sq) : (sq - candidate);

        /* Accept if diff is also prime */
        if (diff >= 2U && is_prime_u64(diff)) {
            if (terms_cnt == terms_cap) {
                terms_cap *= 2U;
                uint64_t *tmp = (uint64_t *)realloc(terms, terms_cap * sizeof(uint64_t));
                if (!tmp) abort();
                terms = tmp;
            }
            terms[terms_cnt++] = candidate;
        }
        ++candidate;   /* next integer to test */
    }

    /* -- Return the requested term -------------------------------------- */
    return (seq_t)terms[n - 1U];
}
