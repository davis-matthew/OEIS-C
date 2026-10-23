/*
 * A163316  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=3*a(n-1)+3*a(n-2)+3*a(n-3)+3*a(n-4)-6*a(n-5)
   (with +1 added for n==5).
   Initial values: a(0)=1, a(1)=5, a(2)=20, a(3)=80, a(4)=320, a(5)=1270 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored values */
    static idx_t cap   = 0;       /* capacity of cache array */
    static idx_t maxn  = 0;       /* highest index whose value is in cache */

    /* Initial values for the first six indices (indices 0..5).   */
    static const seq_t init_tbl[6] = {
        (seq_t)1,  (seq_t)5,  (seq_t)20,  (seq_t)80,  (seq_t)320,  (seq_t)1270
    };

    /* First call – allocate at least enough space for the requested
       index and for the first six initial values.                 */
    if (cache == NULL) {
        size_t needed = (n > 5 ? (size_t)n + 1 : 6U);
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init_tbl[i];
        cap   = needed;
        maxn  = 5;          /* values 0..5 are now known */
    }

    /* If more space is required for the requested index, extend it. */
    if ((size_t)n + 1 > (size_t)cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* If the requested index is already computed, return it. */
    if (n <= maxn) return cache[n];

    /* Compute any missing terms from maxn+1 up to n. */
    for (idx_t i = maxn + 1; i <= n; ++i) {
        /* For indices 0..5 the values are already set.  Only compute
           for i >= 6. */
        if (i <= 5) continue;

        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];

        /* sum = a1 + a2 + a3 + a4          */
        seq_t sum = a1;
        sum += a2;
        sum += a3;
        sum += a4;

        /* sum = 3 * sum  (using only additions) */
        seq_t temp = sum;   /* original sum */
        sum += sum;        /* *2 */
        sum += temp;       /* *3 */

        /* six5 = 6 * a5  (using only additions) */
        seq_t six5 = a5;
        six5 += a5;    /* 2 * a5 */
        six5 += a5;    /* 3 * a5 */
        six5 += a5;    /* 4 * a5 */
        six5 += a5;    /* 5 * a5 */
        six5 += a5;    /* 6 * a5 */

        cache[i] = sum - six5;
    }

    maxn = n;
    return cache[n];
}
