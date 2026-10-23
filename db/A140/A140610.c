/*
 * A140610  Running prime totals of prime factors (without multiplicity) of
 *          consecutive composite N.
 * https://oeis.org/A140610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th running sum of distinct prime factors of
 *            consecutive composite integers that is itself prime
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime, is_prime_big, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;          /* undefined offset */

    /* static cache for computed terms */
    static seq_t *terms   = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    /* running state */
    static idx_t  next_comp   = 4;   /* first composite */
    static seq_t running_sum  = 0;   /* running sum of distinct prime factors */

    /* allocate terms array on first call */
    if (!terms) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;   /* allocation failed */
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* skip over primes */
        while (is_prime(next_comp))
            ++next_comp;

        /* sum distinct prime factors of next_comp */
        uint64_t x = (uint64_t)next_comp;
        seq_t sum_dist = 0;
        while (x > 1) {
            uint64_t p = smallest_prime_factor(x);
            sum_dist += (seq_t)p;
            while (x % p == 0)
                x /= p;
        }

        /* update the running sum */
        running_sum += sum_dist;

        /* record a term if the running sum itself is prime */
        if (is_prime_big(running_sum)) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* realloc failed */
                terms = tmp;
            }
            terms[terms_len++] = running_sum;
        }

        /* advance to the next integer */
        ++next_comp;
    }

    return terms[n - 1];
}
