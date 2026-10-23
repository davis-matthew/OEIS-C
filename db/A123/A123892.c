/*
 * A123892  Expansion of g.f.:
 *          (1+x^2)*(1+2*x^2)/(1-3*x+3*x^2-6*x^3+2*x^4-2*x^5).
 * https://oeis.org/A123892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 6*a(n-3) - 2*a(n-4) + 2*a(n-5);
 *            Initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=24, a(4)=63
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t   */
#include <stdlib.h>   /* malloc, realloc */

/* Static cache for memoisation */
static seq_t *seq      = NULL;   /* array of computed terms */
static size_t seq_cap  = 0;      /* allocated capacity */
static size_t seq_len  = 0;      /* number of terms already computed */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (seq == NULL) {
        seq_cap = 5;
        seq_len = 5;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq == NULL) {        /* allocation failure – abort */
            exit(1);
        }
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)9;
        seq[3] = (seq_t)24;
        seq[4] = (seq_t)63;
    }

    /* requested index already computed? */
    if ((size_t)n < seq_len)
        return seq[(size_t)n];

    /* grow the table if necessary */
    if ((size_t)n >= seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap <= seq_cap)              /* overflow protection */
            new_cap = (size_t)n + 1;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (seq == NULL) {                  /* allocation failure – abort */
            exit(1);
        }
        seq_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 3*a(i-2) + 6*a(i-3) - 2*a(i-4) + 2*a(i-5) */
        seq_t term1 = seq[i - 1];
        term1 *= (seq_t)3;

        seq_t term2 = seq[i - 2];
        term2 *= (seq_t)3;

        seq_t term3 = seq[i - 3];
        term3 *= (seq_t)6;

        seq_t term4 = seq[i - 4];
        term4 *= (seq_t)2;

        seq_t term5 = seq[i - 5];
        term5 *= (seq_t)2;

        seq[i] = term1 - term2 + term3 - term4 + term5;
    }

    seq_len = (size_t)n + 1;
    return seq[(size_t)n];
}
