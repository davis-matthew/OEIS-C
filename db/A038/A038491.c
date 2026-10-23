/*
 * A038491  Sums of 3 distinct powers of 11.
 * https://oeis.org/A038491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 119, u64 n <= 968,
 *            u128 n <= 8330, big any n
 * Helpers:   needed_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038491:  a(n) is the n-th smallest integer that is a sum of three distinct
   powers of 11.  The sequence is the sorted list of 11^i+11^j+11^k
   with 0 <= i < j < k.  The first few terms are
   133, 1343, 1453, 1463, 14653, …  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/needed_exp.h"

static seq_t *powers    = NULL;   /* powers[e] = 11^e */
static size_t pow_len   = 0;      /* number of powers stored   */
static size_t pow_cap   = 0;      /* capacity of powers array  */

static seq_t *terms     = NULL;   /* cached sequence terms     */
static size_t terms_len = 0;      /* number of terms stored    */
static size_t terms_cap = 0;      /* capacity of terms array   */

static idx_t  max_exp_computed = 0; /* largest exponent whose terms are produced */

/* Ensure that powers[] and terms[] contain at least (n+1) sequence terms. */
static void ensure_cache(idx_t n)
{
    size_t required_exp = needed_exp(n);

    /* Initialise powers array if empty */
    if (pow_len == 0) {
        pow_cap = 4;
        powers = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        powers[0] = (seq_t)1;
        pow_len = 1;
    }

    /* Initialise terms array if empty */
    if (terms_cap == 0) {
        terms_cap = 4;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Grow powers array if needed */
    if (pow_len < required_exp + 1) {
        if (pow_cap < required_exp + 1) {
            pow_cap = required_exp + 1;
        }
        powers = (seq_t *)realloc(powers, pow_cap * sizeof(seq_t));
    }

    /* Grow terms array if needed */
    if (terms_cap < required_exp * required_exp) {
        terms_cap = required_exp * required_exp;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }

    /* Generate powers and terms up to required_exp */
    while (max_exp_computed < required_exp) {
        ++max_exp_computed;                     /* new exponent */
        /* Compute next power of 11 */
        powers[max_exp_computed] = powers[max_exp_computed - 1] * (seq_t)11;
        ++pow_len;

        /* Append all sums with this new maximum exponent.
           Order: j outer loop (1 .. max_exp_computed-1),
           i inner loop (0 .. j-1).                            */
        for (size_t j = 1; j < max_exp_computed; ++j) {
            for (size_t i = 0; i < j; ++i) {
                seq_t term = powers[max_exp_computed] + powers[j] + powers[i];
                if (terms_len >= terms_cap) {             /* grow if necessary */
                    terms_cap = (terms_cap == 0) ? 4 : terms_cap * 2;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = term;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 0) return 0;                  /* safety for unsigned underflow */
    ensure_cache(n);
    return terms[(size_t)n];
}
