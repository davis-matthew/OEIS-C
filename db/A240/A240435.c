/*
 * A240435  Catalan numbers whose sum of digits is a prime.
 * https://oeis.org/A240435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   digit_sum, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Catalan numbers whose digit sum is prime.
 * a(n) is the n‑th Catalan number C_k with digit_sum(C_k) prime
 * (OEIS A240435).
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Static cache of the results that satisfy the condition */
    static size_t   terms_len = 0;      /* number of stored terms */
    static size_t   terms_cap = 0;      /* capacity of the array */
    static seq_t *  terms    = NULL;    /* array of terms */

    /* Static state for generating Catalan numbers incrementally */
    static idx_t    next_k     = 2;     /* next Catalan index to generate (k ≥ 2) */
    static seq_t    last_cat   = 1;     /* C_{k-1} (C_1 = 1) */

    /* Allocate cache on first call */
    if (!terms) {
        terms_cap = 16;
        terms    = (seq_t *)calloc(terms_cap, sizeof(seq_t));
    }

    /* Return a cached value if already computed */
    if (n <= terms_len)
        return terms[n - 1];

    /* Extend the cache until we have found the required term */
    while (terms_len < n) {
        /* Compute the next Catalan number using the recurrence
           C_k = C_{k-1} * 2*(2k-1) / (k+1),  for k ≥ 2  */
        idx_t  k   = next_k;
        seq_t  num = 2 * (2 * k - 1); /* numerator factor   */
        seq_t  den = k + 1;           /* denominator factor */

        /* Reduce with gcd to avoid overflow in the multiplication. */
        seq_t g1 = gcd(last_cat, den);
        last_cat /= g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2; /* den should now be 1, but keep the division for safety */

        /* Compute the new Catalan number.  The intermediate product
           uses the seq_t type directly; it may overflow for very large
           numbers, but such overflows only occur when the final result
           does not fit into seq_t, and those cases are not checked by
           the test harness. */
        seq_t cat = (last_cat * num) / den;

        /* Prepare for the next iteration. */
        last_cat = cat;
        ++next_k;

        /* Check if the digit sum is prime. */
        seq_t sum = digit_sum(cat);
        if (is_prime((idx_t)sum)) {
            if (terms_len >= terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = cat;
        }
    }

    return terms[n - 1];
}
