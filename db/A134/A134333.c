/*
 * A134333  Numbers n whose number of prime factors (counted with
 *          multiplicity) is a prime factor of n.
 * https://oeis.org/A134333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134333: 
   Numbers n whose number of prime factors (counted with multiplicity) 
   is a prime factor of n. */
/* a(1) = 4, a(2) = 6, a(3) = 10, a(4) = 12, a(5) = 14, ... */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_prime_factors.h"
#include "../utils/is_prime.h"

static seq_t *terms = NULL;          /* cached terms */
static size_t  terms_capacity = 0;   /* allocated capacity of terms */
static size_t  terms_size    = 0;    /* number of terms stored */
static seq_t  next_candidate  = 2;   /* next integer to test */

/* Ensure that the terms array has at least need entries of capacity. */
static void ensure_capacity(size_t need)
{
    if (terms_capacity < need) {
        size_t new_cap = terms_capacity ? terms_capacity << 1 : 16;
        if (new_cap < need) new_cap = need;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_capacity = new_cap;
    }
}

/* Compute the nth term (1‑based) of A134333. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* should not happen, but safe */

    /* Generate terms until we reach the requested index. */
    while (terms_size < (size_t)n) {
        seq_t m = next_candidate++;
        seq_t om = count_prime_factors(m);   /* Ω(m), multiplicity counted */

        /* Check: Ω(m) > 1, Ω(m) must be prime, and Ω(m) | m */
        if (om > 1) {
            if ((m % om) == 0) {                     /* divisor test */
                if (is_prime((idx_t)om)) {            /* prime test */
                    ensure_capacity(terms_size + 1);
                    terms[terms_size++] = m;
                }
            }
        }
    }
    return terms[n - 1];
}
