/*
 * A155474  Odd nonprimes n with distinct smallest digits of n and n-th prime.
 * https://oeis.org/A155474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth odd nonprime number n such that the smallest
 *            decimal digit of n and the nth prime have different smallest
 *            digits
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime.h"    /* bool is_prime(idx_t x); */

/* Smallest decimal digit of x.  Works for any unsigned seq_t. */
static inline seq_t smallest_digit(seq_t x)
{
    seq_t min = (seq_t)9;
    while (x > 0) {
        seq_t d = x % (seq_t)10;
        if (d < min) min = d;
        x /= (seq_t)10;
    }
    return min;
}

/* Cache for the computed terms.  terms[0] holds a(1). */
static seq_t *terms = NULL;
static size_t terms_size = 0;   /* number of terms stored so far */
static size_t terms_cap  = 0;   /* allocated capacity (in elements) */

/* Last integer that was fully processed (odd nonprime). */
static idx_t last_checked = 0;

/* Return the n‑th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard anyway */

    /* Grow the cache until it contains at least n terms. */
    while (terms_size < (size_t)n) {
        /* Find the next odd integer. */
        do { ++last_checked; } while ((last_checked & 1) == 0); /* skip evens */

        /* Skip primes – we need odd non‑primes only. */
        if (is_prime(last_checked)) continue;

        /* Compare smallest digits of the candidate and the n‑th prime. */
        seq_t d1 = smallest_digit((seq_t)last_checked);
        seq_t p  = (seq_t)get_prime(last_checked);  /* p = prime(last_checked) */
        seq_t d2 = smallest_digit(p);

        if (d1 != d2) {
            /* Store this term.  Grow the array if required. */
            if (terms_size + 1 > terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) abort();   /* memory allocation failure */
                terms = tmp;
            }
            terms[terms_size++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
