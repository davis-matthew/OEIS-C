/*
 * A233578  n >= 2 such that the denominator/6 of Bernoulli(n) is congruent to
 *          {1, 5, 7, 13 or 19} modulo 30.
 * https://oeis.org/A233578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = nth even integer ≥2 for which the product of all primes
   p>3 with p-1 dividing n, taken modulo 30, is in {1,5,7,13,19}. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t *terms      = NULL;   /* array of computed terms */
static size_t terms_cap  = 0;       /* current allocation size  */
static idx_t  terms_cnt  = 0;       /* number of stored terms      */
static idx_t  last_m     = 1;       /* last n processed           */

/* allowed residues modulo 30 */
static const seq_t allowed[5] = {1, 5, 7, 13, 19};

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for n=0; harmless */

    while (terms_cnt < n) {
        ++last_m;                     /* candidate n */

        /* consider only even n ≥ 2 */
        if (last_m < 2 || (last_m & 1))
            continue;

        /* product of primes p>3 with p-1 dividing last_m, modulo 30 */
        seq_t prod = 1;
        for (idx_t i = 1; i * i <= last_m; ++i) {
            if (last_m % i == 0) {
                idx_t d1 = i;
                idx_t d2 = last_m / i;

                idx_t p = d1 + 1;
                if (p > 3 && is_prime_small((seq_t)p))
                    prod = (seq_t)((prod * (p % 30)) % 30);

                if (d1 != d2) {
                    p = d2 + 1;
                    if (p > 3 && is_prime_small((seq_t)p))
                        prod = (seq_t)((prod * (p % 30)) % 30);
                }
            }
        }

        /* check against allowed residues */
        bool ok = false;
        for (int k = 0; k < 5; ++k) {
            if (prod == allowed[k]) {
                ok = true;
                break;
            }
        }

        if (ok) {
            /* grow terms array if necessary */
            if (terms_cnt >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms)
                    exit(EXIT_FAILURE);
                terms      = new_terms;
                terms_cap  = new_cap;
            }
            terms[terms_cnt++] = last_m;
        }
    }

    return terms[n - 1];
}
