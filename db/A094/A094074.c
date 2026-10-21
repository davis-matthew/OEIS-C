/*
 * A094074  Coefficients arising in combinatorial field theory.
 * https://oeis.org/A094074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5 Recurrence: a(n+1) = (2n+1)*((4n+5)*a(n) -
 *            2n*(2n-1)^2*a(n-1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for the sequence values */
static seq_t *seq_cache     = NULL;   /* dynamically grown array */
static size_t  cache_size   = 0;      /* allocated length of seq_cache   */
static size_t  last_index   = 0;      /* highest index that has been computed */

/* Grow the cache so that it can hold indices up to 'need-1' */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_size)
        return;
    seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
    if (new_cache == NULL)
        exit(1);                     /* out of memory */
    if (seq_cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = seq_cache[i];
        free(seq_cache);
    }
    seq_cache   = new_cache;
    cache_size  = need;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Initialise cache on first call */
    if (seq_cache == NULL) {
        ensure_cache_size(2);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)5;
        last_index   = 1;
    }

    /* Compute any missing terms up to n */
    if ((size_t)n + 1 > cache_size) {
        ensure_cache_size((size_t)n + 1);
        for (size_t i = last_index + 1; i <= (size_t)n; ++i) {
            /* i corresponds to (n+1) in the recurrence */
            idx_t nn = (idx_t)(i - 1);   /* n in recurrence */

            /* term1 = (4n+5)*a[n] */
            seq_t term1 = seq_cache[i-1] * (seq_t)(4 * nn + 5);

            /* temp  = 2n*(2n-1)^2*a[n-1] */
            seq_t temp = seq_cache[i-2] *
                         ((seq_t)(2 * nn) *
                          ((seq_t)(2 * nn - 1) *
                           (seq_t)(2 * nn - 1)));

            /* a[n+1] = (2n+1)*(term1 - temp) */
            seq_cache[i] = (seq_t)(2 * nn + 1) *
                           (term1 - temp);
        }
        last_index = (size_t)n;
    }

    return seq_cache[n];
}
