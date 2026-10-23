/*
 * A141274  Composite numbers k such that k+1 is prime and the index in the
 *          composites is also prime.
 * https://oeis.org/A141274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 683, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141274: composite numbers k such that k+1 is prime and
   the index of k among all composites (starting at 0 for 4) is also
   prime (offset 1) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t) */
#include "../utils/seq_realloc.h"    /* seq_t* seq_realloc(seq_t*,size_t,size_t) */

seq_t a(idx_t n)
{
    /* memoised terms */
    static seq_t *terms   = NULL;
    static size_t   len   = 0;    /* number of terms stored */
    static size_t   cap   = 0;    /* capacity of the array */

    /* state for generating terms */
    static idx_t next_num   = 4;  /* next integer to test */
    static idx_t comp_index = 0;  /* index of current composite */

    if (n == 0)                    /* undefined for n=0; safe value */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (len < (size_t)n) {
        idx_t curr = next_num;

        /* skip primes: find next composite */
        while (is_prime(curr))
            ++curr;

        /* curr is composite; comp_index is its index among composites */
        if (is_prime(comp_index) &&
            is_prime((idx_t)(curr + 1))) {   /* k+1 is prime */

            /* grow the array if necessary */
            if (len == cap) {
                size_t new_cap = cap ? 2 * cap : 4;
                seq_t *tmp = seq_realloc(terms, cap, new_cap);
                if (!tmp)
                    abort();            /* allocation failure */
                terms = tmp;
                cap   = new_cap;
            }

            terms[len++] = (seq_t)curr;        /* store the term */
        }

        /* advance to next composite */
        ++comp_index;
        next_num = curr + 1;
    }

    return terms[n - 1];
}
