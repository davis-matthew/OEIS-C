/*
 * A227555  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having nonzero determinant, with
 *          rows and columns of the latter in lexicographically nondecreasing
 *          order.
 * https://oeis.org/A227555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 27, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 66*a(n-2) + 220*a(n-3) - 495*a(n-4)
   + 792*a(n-5) - 924*a(n-6) + 792*a(n-7) - 495*a(n-8) + 220*a(n-9)
   - 66*a(n-10) + 12*a(n-11) - a(n-12)  for n > 12.
   Initial terms: a(1)=7, a(2)=33, a(3)=147, a(4)=585, a(5)=2080,
   a(6)=6653, a(7)=19356, a(8)=51827, a(9)=129090, a(10)=301882,
   a(11)=668004, a(12)=1407882. */

#include <stdlib.h>

/* ---------------------------------------------------------------------- */
/* Cache for computed values.  The array stores a(1) at index 0.         */
/* cache_len holds the number of elements currently initialised.        */
/* cache_cap is the allocated capacity (in elements).                  */
/* ---------------------------------------------------------------------- */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ---------------------------------------------------------------------- */
/* First 12 terms of the sequence (zero‑based index).                    */
/* ---------------------------------------------------------------------- */
static const seq_t init_vals[12] = {
    (seq_t)7,  (seq_t)33,  (seq_t)147, (seq_t)585,
    (seq_t)2080, (seq_t)6653, (seq_t)19356, (seq_t)51827,
    (seq_t)129090, (seq_t)301882, (seq_t)668004, (seq_t)1407882
};

/* ---------------------------------------------------------------------- */
/* Return a(n).  For n = 0 the function returns 0 (offset is 1).  The     */
/* function is safe to call in any order of n; values are memoised.      */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined below offset */

    /* Convert to zero‑based index */
    size_t idx = (size_t)(n - 1);

    /* Allocate or grow the cache if necessary */
    if (cache == NULL || idx >= cache_cap) {
        size_t newcap = (cache == NULL) ? 32 : cache_cap * 2;
        while (newcap <= idx)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)                           /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        cache_cap = newcap;
    }

    /* Initialise the first 12 terms once */
    if (cache_len == 0) {
        for (int i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_len = 12;
    }

    /* Return if already computed */
    if (idx < cache_len)
        return cache[idx];

    /* Compute missing terms up to idx */
    for (size_t k = cache_len; k <= idx; ++k) {
        /* recurrence: a(k+1) depends on a(k), …, a(k-11) */
        seq_t res = cache[k-1] * (seq_t)12;
        res -= cache[k-2] * (seq_t)66;
        res += cache[k-3] * (seq_t)220;
        res -= cache[k-4] * (seq_t)495;
        res += cache[k-5] * (seq_t)792;
        res -= cache[k-6] * (seq_t)924;
        res += cache[k-7] * (seq_t)792;
        res -= cache[k-8] * (seq_t)495;
        res += cache[k-9] * (seq_t)220;
        res -= cache[k-10] * (seq_t)66;
        res += cache[k-11] * (seq_t)12;
        res -= cache[k-12] * (seq_t)1;
        cache[k] = res;
    }

    cache_len = idx + 1;
    return cache[idx];
}
