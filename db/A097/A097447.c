/*
 * A097447  Primes in the concatenation of consecutive prime-sided isosceles
 *          triangles.
 * https://oeis.org/A097447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   append_candidate, concat_parts, digit_count, is_prime,
 *            is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A097447: odd prime-sided isosceles triangles
   (x,y,y) where x is prime and y is the next prime after x.
   For each such pair concatenate the three sides to form a
   decimal number and output the one that is prime.
   The sequence is indexed from 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* is_prime(uint64_t)  */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t) */
#include "../utils/concat_parts.h"  /* concat_parts(idx_t,idx_t) */
#include "../utils/pow10.h"         /* pow10(idx_t) */
#include "../utils/digit_count.h"   /* digit_count(uint64_t) */
#include "../utils/append_candidate.h" /* append_candidate(seq_t**,size_t*,size_t*,seq_t) */

static inline uint64_t next_prime_after(uint64_t n)
{
    /* Return the first prime greater than n.
       Uses the 64‑bit is_prime routine. */
    uint64_t cand = n + 1;
    if (cand <= 2)
        return 2;
    if ((cand & 1ULL) == 0)
        cand++;               /* make odd */
    for (;;) {
        if (is_prime(cand))
            return cand;
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    /* static state to cache computed terms and prime pairs */
    static seq_t *terms = NULL;     /* cached values */
    static size_t terms_len = 0;    /* number of cached terms */
    static size_t terms_cap = 0;    /* allocated capacity */
    static uint64_t prev_prime = 0;
    static uint64_t next_prime = 0;
    static bool initialized = false;

    /* initialize first prime pair on first call */
    if (!initialized) {
        prev_prime = 2;
        next_prime = next_prime_after(prev_prime); /* 3 */
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* concatenate prev_prime and next_prime: prev||next */
        seq_t xy = concat_parts(prev_prime, next_prime);

        /* compute factor of 10^{digits(next_prime)} */
        seq_t ten_pow = pow10(digit_count(next_prime));

        /* final term: (prev||next)*10^{digits(next_prime)} + next_prime
           which is prev||next||next */
        seq_t term = xy * ten_pow + (seq_t)next_prime;

        /* keep it if the concatenation is prime */
        if (is_prime_big(term))
            append_candidate(&terms, &terms_len, &terms_cap, term);

        /* advance to the next consecutive prime pair */
        prev_prime = next_prime;
        next_prime = next_prime_after(next_prime);
    }

    /* return the requested term (1‑based) */
    return terms[n - 1];
}
