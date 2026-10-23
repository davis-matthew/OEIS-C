/*
 * A164068  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 561*a(n-6) +
 *            C_n, where C_0=1, C_1..C_5=2, C_6=1, C_n=0 for n>6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All arithmetic uses the type seq_t, which may be an unsigned builtin or
   an arbitrary-precision integer class. */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed values */
static idx_t  seq_cache_size = 0; /* number of entries in seq_cache */

/* Return the a(n) value of sequence A164068. */
seq_t a(idx_t n)
{
    /* Ensure the cache has room for indices up to n. */
    size_t new_size = (size_t)n + 1;
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else if (new_size > (size_t)seq_cache_size) {
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – unlikely in tests */
        seq_cache = tmp;
    }

    /* Compute values that are not yet cached. */
    for (idx_t i = seq_cache_size; i <= n; ++i) {
        seq_t term = 0;

        /* Compute 33 * (a(i-1)+...+a(i-5)) */
        seq_t sum5 = 0;
        if (i >= 1) sum5 += seq_cache[i-1];
        if (i >= 2) sum5 += seq_cache[i-2];
        if (i >= 3) sum5 += seq_cache[i-3];
        if (i >= 4) sum5 += seq_cache[i-4];
        if (i >= 5) sum5 += seq_cache[i-5];
        sum5 = sum5 * ((seq_t)33);
        term = sum5;

        /* Subtract 561 * a(i-6) if i>=6 */
        if (i >= 6) {
            term = term - seq_cache[i-6] * ((seq_t)561);
        }

        /* Add the constant term C_i */
        if (i == 0)          term += 1;
        else if (i <= 5)     term += 2;
        else if (i == 6)     term += 1; /* for i>6, C_i = 0 */

        seq_cache[i] = term;
    }

    seq_cache_size = n + 1;
    return seq_cache[n];
}
