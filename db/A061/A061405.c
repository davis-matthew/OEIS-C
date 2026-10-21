/*
 * A061405  Denominators of coefficients of even powers of x of the expansion
 *          of Bessel function J_6(x).
 * https://oeis.org/A061405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+6) * n! * (n+6)!
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised factorial array */
static seq_t *fact = NULL;
static idx_t fact_size = 0;   /* number of factorials stored (fact[0] = 0!) */
static idx_t fact_cap  = 0;   /* allocated capacity */

/* Ensure we have factorials up to index 'n' (inclusive) */
static void init_fact_up_to(idx_t n)
{
    if (n < fact_size)
        return;                      /* already have enough */

    idx_t needed = n + 1;            /* elements 0 .. n */

    /* Allocate or grow the array if necessary */
    if (needed > fact_cap) {
        idx_t new_cap = fact_cap ? fact_cap * 2 : 16;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact = tmp;
        fact_cap = new_cap;
    }

    /* Initialise the factorial array if it hasn't been initialized yet */
    if (fact_size == 0) {
        fact[0] = (seq_t)1;    /* 0! = 1 */
        fact_size = 1;
    }

    /* Compute remaining factorials */
    for (idx_t i = fact_size; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_size = n + 1;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    /* We need factorials up to n+6 */
    init_fact_up_to(n + 6);

    /* Compute 2^(2*n+6) by repeated multiplication */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < 2 * n + 6; ++i) {
        res *= (seq_t)2;
    }

    /* Multiply by n! and (n+6)! */
    res *= fact[n];
    res *= fact[n + 6];

    return res;
}
