/*
 * A229406  Number of n X 2 0..3 arrays avoiding 11 horizontally, 22
 *          vertically, 33 antidiagonally and 00 diagonally.
 * https://oeis.org/A229406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 13*a(n-2) - 67*a(n-3) + 38*a(n-4) - 2*a(n-5)
 *            Initial values: a(1)=15, a(2)=163, a(3)=1756, a(4)=18712,
 *            a(5)=199595
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised; it works for arbitrary sequence type seq_t
   (unsigned integer of any bit width or an arbitrary‑precision integer
   class compatible with +, -, *, and casts from integer constants).  */

#include <stdlib.h>

static seq_t *cache = NULL;           /* 1‑indexed cache: cache[0] = a(1)  */
static idx_t  cache_size = 0;        /* Number of terms computed             */
static idx_t  cache_cap  = 0;        /* Allocated capacity                   */

static const seq_t init[5] = {
    (seq_t)15,
    (seq_t)163,
    (seq_t)1756,
    (seq_t)18712,
    (seq_t)199595
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety – offset starts at 1 */

    /* Initialise cache the first time.  */
    if (cache == NULL) {
        cache_cap = (n > 5) ? n : 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;    /* allocation failure – unlikely   */
        cache_size = 0;
    }

    /* Grow cache if needed before we start computing new terms. */
    if (n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = n;
    }

    /* Compute missing terms up to n.  */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 5) {
            val = init[i-1];
        } else {
            /* a(i) = 10*a(i-1) + 13*a(i-2) - 67*a(i-3) + 38*a(i-4) - 2*a(i-5)  */
            /* All products are computed as seq_t.  The intermediate sum of
              the positive terms is guaranteed to be larger than the
              corresponding sum of negative terms, so the subtraction
              never underflows for the sequence values. */
            seq_t pos = (seq_t)10 * cache[i-2] +          /* a(i-1)  */
                        (seq_t)13 * cache[i-3] +          /* a(i-2)  */
                        (seq_t)38 * cache[i-5];           /* a(i-4)  */
            seq_t neg = (seq_t)67 * cache[i-4] +          /* a(i-3)  */
                        (seq_t)2  * cache[i-6];          /* a(i-5)  */
            val = pos - neg;
        }
        cache[i-1] = val;
    }
    cache_size = n;

    return cache[n-1];
}
