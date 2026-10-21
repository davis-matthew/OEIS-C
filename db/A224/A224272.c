/*
 * A224272  G.f. satisfies: A(x) = (A(x^2) + x)^2.
 * https://oeis.org/A224272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 127, u64 n <= 447,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 2
   for n >= 0:
       a(2*n + 1) = 2 * a(n)
       a(2*n)     = Σ_{k=0}^{n} a(k) * a(n - k) + (n == 1 ? 1 : 0)
*/

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (seq_cache_cap > n) return;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;

    /* Initialise the first two slots if needed */
    if (new_cap > 0) seq_cache[0] = (seq_t)1;
    if (new_cap > 1) seq_cache[1] = (seq_t)2;

    /* Compute new entries */
    idx_t start = seq_cache_cap;
    if (start < 2) start = 2;
    for (idx_t i = start; i <= n; ++i)
    {
        if (i & 1) /* odd: a(2*m+1) = 2*a(m) */
        {
            idx_t m = (i - 1) >> 1;
            seq_cache[i] = (seq_t)2 * seq_cache[m];
        }
        else /* even: a(2*m) */
        {
            idx_t m = i >> 1;
            seq_t sum = 0;
            for (idx_t k = 0; k <= m; ++k)
                sum += seq_cache[k] * seq_cache[m - k];
            if (i == 2) sum += (seq_t)1; /* the x^2 term */
            seq_cache[i] = sum;
        }
    }
    seq_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
