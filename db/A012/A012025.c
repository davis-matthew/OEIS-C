/*
 * A012025  E.g.f. arcsinh(sin(arctan(x))) = arcsinh(x/(1+x^2)^(1/2)) (odd
 *          powers only).
 * https://oeis.org/A012025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^{n-1} * (2n-2)! * \sum_{k=0}^{n-1} C(2k,k)/2^k
 *            (recurrence: a(1)=1,
 *            a(n)=-(2n-2)(2n-3)a(n-1)+(-1)^{n-1}[(2n-2)!]^2/((n-1)!^2·2^{n-1})
 *            ) computed with memoisation and factorial caching
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t *fact_arr   = NULL;   /* factorial array: fact_arr[i] = i!   */
static size_t fact_cap  = 0;       /* current allocated size for factorials */

/* static cache for sequence values */
static seq_t *seq_arr   = NULL;   /* seq_arr[i] = a(i), i>=1                */
static size_t seq_cap  = 0;       /* current allocated size for seq_arr     */

/* enlarge factorial array up to index upto (inclusive) */
static void ensure_factorial(idx_t upto)
{
    if ((size_t)upto + 1 <= fact_cap) return;
    size_t new_cap = (size_t)upto + 1;
    seq_t *tmp = (seq_t *)realloc(fact_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    fact_arr = tmp;
    if (fact_cap == 0) {
        fact_arr[0] = (seq_t)1;
        fact_cap = 1;
    }
    for (size_t i = fact_cap; i < new_cap; ++i) {
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
    }
    fact_cap = new_cap;
}

/* enlarge sequence cache up to n (inclusive) and compute missing values */
static void ensure_sequence(idx_t n)
{
    if ((size_t)n + 1 <= seq_cap) return;
    size_t old_cap = seq_cap;
    seq_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_arr = tmp;
    /* compute values from old_cap to n */
    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        if (i == 0) {
            seq_arr[0] = (seq_t)0;          /* unused, just to keep indices aligned */
            continue;
        }
        if (i == 1) {
            seq_arr[1] = (seq_t)1;
            continue;
        }
        idx_t ii = (idx_t)i;
        /* ratio = (2i-2)! / (i-1)! */
        idx_t two_off = 2 * ii - 2;
        idx_t off     = ii - 1;
        ensure_factorial(two_off);
        seq_t ratio = fact_arr[two_off] / fact_arr[off];

        idx_t m = ii - 1;          /* m = n-1 */
        idx_t k = m / 2;
        seq_t r = ratio / pow2((idx_t)k);          /* divide by 2^k */
        seq_t term = r * r;                           /* term = r^2 */
        if (m % 2 == 1) term /= (seq_t)2;            /* adjust for odd exponent */

        seq_t prev = seq_arr[i - 1];
        seq_t coeff = (seq_t)(2 * ii - 2) * (seq_t)(2 * ii - 3);
        seq_t sign  = (ii % 2 == 0) ? -(seq_t)1 : (seq_t)1;   /* (-1)^{n-1} */

        seq_t a_val = -coeff * prev + sign * term;
        seq_arr[i] = a_val;
    }
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour for offset <1 unused */
    ensure_sequence(n);
    return seq_arr[n];
}
