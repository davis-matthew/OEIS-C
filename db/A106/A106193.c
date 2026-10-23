/*
 * A106193  Expansion of sqrt(1-4x)/(1-2x^2).
 * https://oeis.org/A106193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n) = (4n-6)*a(n-1) + 2n*a(n-2) - (8n-12)*a(n-3)  for n>=4
 * Initial values: a(0)=1, a(1)=-2, a(2)=0, a(3)=-8
 * Computes arbitrary‑precision integer sequence A106193.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* memoised values */
static idx_t a_size   = 0;       /* number of filled entries in a_cache */
static idx_t a_cap    = 0;       /* allocated capacity */

/* Ensure a_cache can hold indices up to 'n' */
static void a_ensure(idx_t n)
{
    if (n < a_cap) return;
    idx_t new_cap = a_cap ? a_cap * 2 : 8;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp;
    a_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    a_ensure(n);
    if (n < a_size) return a_cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = a_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)            val = 1;
        else if (i == 1)       val = -2;
        else if (i == 2)       val = 0;
        else if (i == 3)       val = -8;
        else {
            /* recurrence: a(i) = ((4i-6)*a(i-1) + 2i*a(i-2) - (8i-12)*a(i-3)) / i */
            seq_t term1 = ((seq_t)4 * (seq_t)i - (seq_t)6) * a_cache[i - 1];
            seq_t term2 = ((seq_t)2 * (seq_t)i)           * a_cache[i - 2];
            seq_t term3 = -(((seq_t)8 * (seq_t)i - (seq_t)12) * a_cache[i - 3]);
            val = (term1 + term2 + term3) / (seq_t)i;
        }
        a_cache[i] = val;
    }
    a_size = n + 1;
    return a_cache[n];
}
