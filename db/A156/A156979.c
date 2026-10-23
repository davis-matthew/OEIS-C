/*
 * A156979  Primes p such that 1 = abs(largest digit of p - sum of all the
 *          other digits of p).
 * https://oeis.org/A156979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that |2*maxdigit(p) - sumdigits(p)|
 *            = 1
 * Range:     u8 n <= 12, u16 n <= 691, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_condition, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* external utilities used */
#include "../utils/next_prime.h"
#include "../utils/digit_condition.h"

static seq_t  *terms = NULL;
static size_t terms_cap = 0;
static idx_t  terms_len = 0;
static uint64_t last_prime = 1U;   /* value before the first prime */

/* ensure capacity of terms array */
static void ensure_terms_capacity(void)
{
    if (terms_len + 1 > terms_cap) {
        size_t new_cap = terms_cap == 0 ? 64 : terms_cap << 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* simple error handling */
        terms = tmp;
        terms_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* generate terms until we have at least n terms */
    while (terms_len < n) {
        /* fetch next prime after last_prime */
        uint64_t p = next_prime(last_prime + 1U);
        last_prime = p;
        if (digit_condition(p)) {
            ensure_terms_capacity();
            terms[terms_len] = (seq_t)p;
            terms_len++;
        }
    }
    return terms[n - 1];
}
