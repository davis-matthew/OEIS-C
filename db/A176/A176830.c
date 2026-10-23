/*
 * A176830  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=4, k=1 and l=-1.
 * https://oeis.org/A176830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{p=0}^{n-1} a(p)*a(n-1-p) + (n-1), with a(0)=1,
 *            a(1)=4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed efficiently via the linear recurrence (valid for n>=5):
   (n+1)a_n +(-7n+2)a_{n-1} +3*(n+1)a_{n-2} +(19n-62)a_{n-3} +4*(-7n+29)a_{n-4} +12*(n-5)a_{n-5} = 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding already computed terms. */
    static seq_t *cache = NULL;
    static idx_t last_computed = 0;     /* highest index already stored in 'cache' */
    static size_t capacity = 0;         /* allocated size of 'cache' */

    /* Initialize base terms if needed. */
    if (cache == NULL) {
        capacity = 5; /* store indices 0..4 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)36;
        cache[4] = (seq_t)147;
        last_computed = 4;
    }

    /* Return cached value if already computed. */
    if (n <= (idx_t)last_computed)
        return cache[n];

    /* Ensure enough space in the cache. */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity ? capacity * 2 : 5;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* For i < 5 the values are already known (base case). */
        if (i < 5)
            continue;

        /* Compute numerator of recurrence: (neg_sum - pos_sum) */
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions: 3*(i+1)*a[i-2]
         *                      +(19*i-62)*a[i-3]
         *                      +12*(i-5)*a[i-5]   (zero for i==5) */
        pos += (seq_t)(3 * (i + 1)) * cache[i - 2];
        pos += (seq_t)(19 * i - 62) * cache[i - 3];
        pos += (seq_t)(12 * (i - 5)) * cache[i - 5];

        /* Negative contributions: (7*i-2)*a[i-1]
         *                       +4*(7*i-29)*a[i-4] */
        neg += (seq_t)(7 * i - 2) * cache[i - 1];
        neg += (seq_t)(4 * (7 * i - 29)) * cache[i - 4];

        /* Compute a[i] = (neg - pos) / (i + 1).  The division is exact. */
        cache[i] = (neg - pos) / (seq_t)(i + 1);
    }

    last_computed = n;
    return cache[n];
}
