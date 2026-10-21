/*
 * A212576  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          |w-x|=2|x-y|+2|y-z|.
 * https://oeis.org/A212576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=a(n-2)+2*a(n-3)+a(n-4)-2*a(n-5)-2*a(n-6)-2*a(n-7)
   +a(n-8)+2*a(n-9)+a(n-10)-a(n-12).  Initial values:
   a(0)=0,a(1)=1,a(2)=2,a(3)=7,a(4)=16,a(5)=33,a(6)=52,
   a(7)=87,a(8)=126,a(9)=181,a(10)=246,a(11)=331.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* memoised values */
    static size_t cap   = 0;        /* allocated capacity (in elements) */
    static idx_t last   = (idx_t)-1;/* highest index computed */

    /* Base values for n = 0 … 11 */
    static const seq_t base_vals[12] = {
        (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)7,
        (seq_t)16, (seq_t)33, (seq_t)52, (seq_t)87,
        (seq_t)126, (seq_t)181, (seq_t)246, (seq_t)331
    };

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();              /* safety */
        for (idx_t i = 0; i < 12; ++i) cache[i] = base_vals[i];
        last = 11;
    }

    /* Return cached value if already computed */
    if (n <= last) return cache[n];

    /* Grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t pos = cache[i-2];
        pos += cache[i-3] * (seq_t)2;
        pos += cache[i-4];
        pos += cache[i-8];
        pos += cache[i-9] * (seq_t)2;
        pos += cache[i-10];

        seq_t neg = cache[i-5] * (seq_t)2;
        neg += cache[i-6] * (seq_t)2;
        neg += cache[i-7] * (seq_t)2;
        neg += cache[i-12];

        cache[i] = pos - neg;
    }
    last = n;
    return cache[n];
}
