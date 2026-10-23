/*
 * A009832  Expansion of e.g.f. tanh(x)*exp(x).
 * https://oeis.org/A009832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for odd n, a(0) = 0, for even n>0, a(n) = 1 - E_n ,
 *            where E_n is the Euler (secant) number, defined by E_0 = 1, E_n
 *            = -∑_{k=0}^{n-1} binom(n,k) * E_k (n≥1). This is the recurrence
 *            derived from the identity ∑_{k=0}^{n} binom(n,k) E_k = 0
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 23, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

/* static cache of Euler numbers E[0..max_n] */
static seq_t *E_cache = NULL;       /* array of Euler numbers */
static idx_t  E_len   = 0;           /* number of stored elements (max index+1) */

/* utility: ensure Euler numbers up to 'max' (inclusive) are stored */
static void
ensure_euler(idx_t max)
{
    if (max < E_len)   /* already have enough */
        return;

    /* grow array to hold indices 0..max */
    seq_t *tmp = (seq_t *)realloc(E_cache, (size_t)(max + 1) * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure: abort */
        abort();
    }
    E_cache = tmp;

    /* initialise if first time */
    if (E_len == 0) {
        E_cache[0] = (seq_t)1;   /* E_0 = 1 */
        E_len = 1;
    }

    /* compute missing Euler numbers */
    for (idx_t n = E_len; n <= max; ++n) {
        if (n & 1) {          /* odd indices are zero */
            E_cache[n] = (seq_t)0;
            continue;
        }

        /* compute E_n = -∑_{k=0}^{n-1} binom(n,k) * E_k   (k step 2) */
        idx_t  m = n / 2;          /* number of terms in the sum (even k from 0 to n-2) */
        seq_t  sum = 0;
        seq_t  bin = 1;            /* binom(n,0) */

        for (idx_t j = 0; j < m; ++j) {
            idx_t k = j * 2;       /* even index */
            sum += bin * E_cache[k];

            /* update binom(n, k+2) for next iteration, if there is one */
            if (j + 1 < m) {
                /* bin = bin * (n-k) * (n-k-1) / ( (k+2)*(k+1) )   */
                bin = bin * (seq_t)(n - k) * (seq_t)(n - k - 1);
                bin = bin / ( (seq_t)(k + 2) * (seq_t)(k + 1) );
            }
        }

        E_cache[n] = -sum;
    }

    E_len = max + 1;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 */

    if (n & 1)                      /* odd n */
        return (seq_t)1;            /* a(odd) = 1 */

    /* even n: a(n) = 1 - E_n */
    ensure_euler(n);                /* make sure E_n is available */
    return ((seq_t)1) - E_cache[n];
}
