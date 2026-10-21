/*
 * A228020  Composite numbers whose initial, all intermediate and final
 *          iterated digit sums are composite numbers.
 * https://oeis.org/A228020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for composite numbers whose initial,
   all intermediate and final iterated digit sums are composite.
   (OEIS A228020) */

#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

/* Return true iff x is composite (x > 1 and not prime). */
static bool is_composite(seq_t x)
{
    return (x > 1) && !is_prime_big(x);
}

/* Return true iff every iterated digit sum of x is composite. */
static bool good_digit_sums(seq_t x)
{
    seq_t cur = x;
    while (1) {
        if (!is_composite(cur))
            return false;
        if (digit_len(cur) == 1)   /* final single‑digit sum */
            break;
        cur = digit_sum(cur);
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms = NULL;
    static size_t term_cnt = 0;   /* number of stored terms */
    static size_t term_cap = 0;   /* capacity of cache */
    static seq_t next = 4;        /* next integer to test */

    /* initialise cache on first use */
    if (!terms) {
        term_cap = 128;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!terms)
            abort();   /* allocation failure */
    }

    /* generate terms until we have at least n of them */
    while (term_cnt < n) {
        /* find next candidate >= next */
        seq_t x = next;
        if (x < 4)
            x = 4;
        /* skip primes */
        while (is_prime_big(x))
            x++;
        /* test digit–sum chain */
        if (good_digit_sums(x)) {
            /* store term */
            if (term_cnt >= term_cap) {
                term_cap *= 2;
                seq_t *new_terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                if (!new_terms)
                    abort();
                terms = new_terms;
            }
            terms[term_cnt++] = x;
        }
        /* set next candidate for following iteration */
        next = x + 1;
    }

    return terms[n - 1];
}
