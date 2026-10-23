/*
 * A106900  Primes of the form x^2+xy+12y^2, with x and y nonnegative.
 * https://oeis.org/A106900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime that can be expressed as p = x^2 + x*y +
 *            12*y^2 with x, y ≥ 0 (discriminant = -47). The routine searches
 *            primes in increasing order and tests the representation
 *            condition
 * Range:     u8 n <= 1, u16 n <= 578, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/integer_sqrt.h"

/* Static state: discovered terms */
static seq_t  *terms      = NULL;   /* array of found primes   */
static idx_t  terms_cap   = 0;      /* allocated capacity      */
static idx_t  terms_cnt   = 0;      /* number of terms stored  */
static uint64_t cur_candidate = 2;  /* next integer to test    */

/* Append a term to the array, resizing if necessary. */
static void push_term(seq_t val)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t*)malloc(sizeof(seq_t) * terms_cap);
    } else if (terms_cnt == terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t*)realloc(terms, sizeof(seq_t) * terms_cap);
    }
    terms[terms_cnt++] = val;
}

/* Generate terms until we have at least n terms. */
static void generate_upto(idx_t n)
{
    if (terms_cnt >= n) return;

    while (terms_cnt < n) {
        uint64_t p = cur_candidate++;
        if (!is_prime(p)) continue;

        bool rep = false;
        for (uint64_t y = 0; ; ++y) {
            uint64_t y2 = y * y;
            uint64_t t  = 12 * y2;
            if (t > p) break;       /* further y only increase t */

            int64_t k2 = (int64_t)4 * p - (int64_t)47 * y2;
            if (k2 < 0) continue;   /* discriminant negative */

            idx_t k_i = integer_sqrt((idx_t)k2);
            seq_t k   = (seq_t)k_i;

            if ((k * k) != (seq_t)k2) continue;   /* not a perfect square */

            if (((k_i - y) & 1UL) != 0) continue;  /* parity condition */

            rep = true;
            break;
        }

        if (rep) push_term((seq_t)p);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n==0 */
    generate_upto(n);
    return terms[n - 1];
}
