/*
 * A025235  a(n) = (1/2)*A014431(n+2).
 * https://oeis.org/A025235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{t=0..floor(n/2)} binom(n, 2t) * 2^t * Catalan(t)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (uses the 0‑based indexing used by OEIS) */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

/* Cache of Catalan numbers */
static seq_t *cat = NULL;
static size_t cat_cap = 0;

/* Ensure that Catalan numbers up to idx u (inclusive) are available */
static void ensure_catalan(idx_t u)
{
    if ((idx_t)u >= (idx_t)cat_cap) {
        size_t new_cap = (size_t)u + 1;          /* at least<|reserved_200494|> u+1 */
        seq_t *tmp = (seq_t *)realloc(cat, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cat = tmp;

        /* Compute missing Catalan values */
        for (size_t i = (cat_cap > 0 ? cat_cap : 0); i <= (size_t)u; ++i) {
            if (i == 0) {
                cat[0] = (seq_t)1;
                continue;
            }
            /* cat[i] = cat[i-1] * (4*(i-1)+2) / (i+1) */
            seq_t num = cat[i - 1] * (seq_t)(4 * (i - 1) + 2);
            cat[i] = num / (seq_t)(i + 1);
        }
        cat_cap = new_cap;
    }
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    /* Largest t needed */
    const idx_t t_max = n >> 1;        /* floor(n/2) */
    ensure_catalan(t_max);

    seq_t result = 0;
    seq_t binom  = 1;                  /* binom(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        if ((k & 1) == 0) {            /* k even -> t = k/2 */
            idx_t t = k >> 1;
            seq_t cat_val = cat[t];
            seq_t pow2t = pow2(t);    /* 2^t */
            seq_t term = binom;
            term = term * pow2t;
            term = term * cat_val;
            result += term;
        }
        if (k == n)
            break;
        /* Update binom for next k:  binom(n,k+1) = binom(n,k)*(n-k)/(k+1) */
        binom = binom * (n - k) / (k + 1);
    }
    return result;
}
