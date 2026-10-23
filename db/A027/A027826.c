/*
 * A027826  Inverse binomial transform of a_0 = 1, a_1, a_2, etc. is a_0, 0,
 *          a_1, 0, a_2, 0, etc.
 * https://oeis.org/A027826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the defining linear relations for the
   inverse‑binomial transform:
     a(0) = 1, a(1) = 1.
   For n ≥ 2 let k = n/2.  Then
     Σ_{i=0}^n (-1)^{n-i} C(n,i) a(i) =
          a(k)   if n is even
          0      if n is odd  .
   Using these equations we solve for a(n) iteratively. */
#include <stdlib.h>

static seq_t  *cache = NULL;   /* sequence cache */
static idx_t  cache_last = 0;  /* last index stored in cache */
static idx_t  cache_cap  = 0;  /* cache capacity */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure cache can hold indices up to n */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap == 0 ? 2 : cache_cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base cases on first call */
    if (cache_last == 0 && cache_cap >= 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_last = 1;
    } else if (cache_last < 1 && cache_cap >= 2) {
        cache[0] = (seq_t)1;
        cache_last = 0;
    }

    /* Return if already computed */
    if (n <= cache_last)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t m = cache_last + 1; m <= n; ++m) {
        seq_t sum_pos = 0, sum_neg = 0;   /* separate positive/negative parts */
        seq_t binom   = 1;                /* C(m,0) */
        int sign = (m % 2 == 0) ? 1 : -1; /* (-1)^m for i = 0 */

        for (idx_t i = 0; i < m; ++i) {
            if (sign > 0)
                sum_pos += binom * cache[i];
            else
                sum_neg += binom * cache[i];

            if (i == m - 1)
                break;

            /* update binomial coefficient to C(m, i+1) */
            binom = binom * (m - i) / (i + 1);
            sign = -sign;   /* flip sign for next i */
        }

        if (m % 2 == 0) {
            /* even: a(m) = a(m/2) + sum_neg - sum_pos */
            cache[m] = cache[m >> 1] + sum_neg - sum_pos;
        } else {
            /* odd: a(m) = sum_neg - sum_pos */
            cache[m] = sum_neg - sum_pos;
        }
    }

    cache_last = n;
    return cache[n];
}
