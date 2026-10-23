/*
 * A122575  Expansion of -x*(1-x)/(1+14*x+x^2)^3.
 * https://oeis.org/A122575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 15, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(1)=-1; a(2)=43; a(3)=-1215; a(4)=28445; a(5)=-597638.
   For n≥0:  a(n) = b(n) - 42*a(n‑1) - 591*a(n‑2) - 2828*a(n‑3) - 591*a(n‑4)
                        - 42*a(n‑5) - a(n‑6),
   where b(n) = -1 if n==1, 1 if n==2, otherwise 0.
   Negative indices are treated as 0.  This follows from the generating
   function G(x) = –x(1–x)/(1+14x+x²)³. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        exit(1);                     /* allocation failure */
    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t bn;
        if (i == 1)
            bn = (seq_t)-1;
        else if (i == 2)
            bn = (seq_t)1;
        else
            bn = (seq_t)0;

        seq_t s = bn;
        if (i >= 1) s -= (seq_t)42   * cache[i - 1];
        if (i >= 2) s -= (seq_t)591  * cache[i - 2];
        if (i >= 3) s -= (seq_t)2828 * cache[i - 3];
        if (i >= 4) s -= (seq_t)591  * cache[i - 4];
        if (i >= 5) s -= (seq_t)42   * cache[i - 5];
        if (i >= 6) s -= (seq_t)1    * cache[i - 6];

        cache[i] = s;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
