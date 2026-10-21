/*
 * A062973  Chowla function of n is not divisible by phi(n).
 * https://oeis.org/A062973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer x such that the sum of all divisors
 *            of x except 1 and x is NOT divisible by φ(x)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   sum_divisors_excl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sum_divisors_excl.h"

/* static storage for memoised terms of the sequence */
static seq_t *terms   = NULL;
static size_t  cap    = 0;
static size_t  len    = 0;
static idx_t   next   = 1;   /* next integer to test */

/* compute Euler's totient φ(n) */
static seq_t phi_of(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            result -= result / p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        result -= result / m;
    return result;
}

/* return the nth term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    /* extend memoised list until we have at least n terms */
    while (len < (size_t)n) {
        seq_t s = sum_divisors_excl(next);
        seq_t p = phi_of(next);
        if (p != 0 && s % p != 0) {  /* x satisfies the condition */
            if (len == cap) {        /* grow the array */
                size_t new_cap = cap ? cap * 2 : 128;
                seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_ptr)
                    abort();          /* allocation failed */
                terms = new_ptr;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)next;
        }
        ++next;
    }
    return terms[n - 1];
}
