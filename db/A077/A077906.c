/*
 * A077906  Expansion of (1-x)^(-1)/(1+x+2*x^3).
 * https://oeis.org/A077906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sum_{k=0}^{n} b(k) where b satisfies b(0) = 1, b(n) =
 *            -b(n‑1) – 2*b(n‑3) for n ≥ 1, with b(m)=0 for m<0. The
 *            generating function of a is 1/(1‑x)/(1+x+2x³)
 * Range:     i8 n <= 11, i16 n <= 21, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

/* static cache of computed terms */
static seq_t *a_cache = NULL;          /* a(n) values   */
static seq_t *b_cache = NULL;          /* b(n) values   */
static idx_t  cache_size = 0;          /* number of entries cached */

static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                      /* already computed */

    idx_t new_size = n + 1;            /* we need indices 0..n inclusive */

    seq_t *ta = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    seq_t *tb = (seq_t*)realloc(b_cache, new_size * sizeof(seq_t));
    if (ta == NULL || tb == NULL)
        abort();                      /* allocation failed */

    a_cache = ta;
    b_cache = tb;

    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0)
        {
            b_cache[0] = (seq_t)1;
            a_cache[0] = b_cache[0];
        }
        else
        {
            seq_t bm3 = (i >= 3) ? b_cache[i-3] : (seq_t)0;
            seq_t bn  = -b_cache[i-1] - (seq_t)2 * bm3;
            b_cache[i] = bn;
            a_cache[i] = a_cache[i-1] + bn;
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
