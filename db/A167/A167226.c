/*
 * A167226  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A167226        */
/*  Recurrence derived from the rational generating function:  */
/*  a(0) = 1;  for n >= 1 */
/*      let p[n] = 1 for n==0, 2 for 1 <= n <= 13, 1 for n==14, 0 otherwise */
/*      let S(n) = a[n-1] + a[n-2] + ... + a[n-13] (sum of the last 13 terms; */
/*      missing terms when index < 0 are treated as 0). */
/*      let T(n) = a[n-14] (0 if n < 14). */
/*      then a[n] = p[n] + 25 * S(n) - 325 * T(n).  For n >= 15, p[n] == 0. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Helper: ensure the cache can hold elements up to index 'new_len' (exclusive). */
static void ensure_cache(size_t new_len)
{
    if (new_len <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Prefilled p[n] for 0 <= n <= 14.  p[n] = 1 (n==0 or 14), 2 otherwise. */
static const seq_t p[15] = {
    (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
    (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
    (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)1
};

seq_t a(idx_t n)
{
    /* convert n to size_t for array indexing */
    size_t nn = (size_t)n;

    /* expand cache if needed */
    if (cache_len <= nn)
    {
        ensure_cache(nn + 1);
        /* compute values from cache_len up to nn */
        for (size_t i = cache_len; i <= nn; ++i)
        {
            /* sum of last 13 terms */
            seq_t sum = (seq_t)0;
            size_t kmax = (i < 13) ? i : 13;
            for (size_t k = 1; k <= kmax; ++k)
                sum += cache[i - k];

            /* term 14 */
            seq_t term14 = (i >= 14) ? cache[i - 14] : (seq_t)0;

            /* p[i] if i <= 14 else 0 */
            seq_t pi = (i < 15) ? p[i] : (seq_t)0;

            /* recurrence: a[i] = pi + 25*sum - 325*term14 */
            seq_t res = pi + (seq_t)25 * sum - (seq_t)325 * term14;
            cache[i] = res;
        }
        cache_len = nn + 1;
    }

    return cache[nn];
}
