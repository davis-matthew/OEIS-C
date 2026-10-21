/*
 * A054211  Numbers k such that k concatenated with k-1 is prime.
 * https://oeis.org/A054211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 3299, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_u64, pow10_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054211: Numbers k such that k concatenated with k-1 is prime.
   Computed by incremental search. The n-th term a(n) is returned. */
/* Dependencies: is_prime_u64 from <../utils/is_prime_u64.h>
                  is_prime_big from <../utils/is_prime_big.h> */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_prime_big.h"
#include "../utils/pow10_digits.h"

static seq_t *terms = NULL;          /* dynamic array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* capacity of the array */
static uint64_t next_k = 4;          /* next candidate k to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined, but safe */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t pow10 = pow10_digits(next_k - 1);
        seq_t concat = k * pow10 + (seq_t)(next_k - 1);

        bool prime;
        if (sizeof(seq_t) <= 8) {
            /* seq_t fits in 64 bits – use the fast routine. */
            prime = is_prime_u64((uint64_t)concat);
        } else {
            prime = is_prime_big(concat);
        }

        if (prime) {
            /* Grow storage if necessary */
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();    /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
