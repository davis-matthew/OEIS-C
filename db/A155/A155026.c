/*
 * A155026  Primes with odd smallest digit.
 * https://oeis.org/A155026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto, smallest_digit_is_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth prime whose smallest decimal digit is odd. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/smallest_digit_is_odd.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t limit = 2000000;          /* initial prime limit */
    static uint64_t *primes = NULL;        /* array of primes up to limit */
    static size_t prime_cnt = 0;
    static seq_t *qual = NULL;             /* array of qualifying primes */
    static size_t qual_cnt = 0;

    /* First call: compute qualifiers up to the initial limit */
    if (!initialized) {
        build_primes_upto(limit, &primes, &prime_cnt);

        qual = (seq_t *)malloc(sizeof(seq_t) * prime_cnt);
        qual_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (smallest_digit_is_odd((seq_t)primes[i]))
                qual[qual_cnt++] = (seq_t)primes[i];
        }
        initialized = true;
    }

    /* If the requested index is beyond what we have, enlarge the limit
       and recompute the qualifiers from scratch. */
    while (n > qual_cnt) {
        /* enlarge the search limit and rebuild */
        limit *= 2;
        free(primes);
        build_primes_upto(limit, &primes, &prime_cnt);

        free(qual);
        qual = (seq_t *)malloc(sizeof(seq_t) * prime_cnt);
        qual_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (smallest_digit_is_odd((seq_t)primes[i]))
                qual[qual_cnt++] = (seq_t)primes[i];
        }
    }

    return qual[n - 1];
}
