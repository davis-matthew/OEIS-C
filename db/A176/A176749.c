/*
 * A176749  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=3, k=0 and l=-1.
 * https://oeis.org/A176749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176749
   a(0) = 1, a(1) = 3, a(2) = 5, a(3) = 18
   For n ≥ 4:
     a(n) = [ 2*(3n−1)*a(n−1) − (n+3)*a(n−2) − 4*(4n−13)*a(n−3) + 12*(n−4)*a(n−4) ] / (n+1)
   (Derived linear recurrence from the OEIS entry.) */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* memoised cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* allocated capacity (number of entries) */
    static idx_t  next_compute = 0;/* next index that needs to be computed */

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;               /* need space for first four terms */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise first four terms only once */
    if (next_compute == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)18;
        next_compute = 4;
    }

    /* if requested value is already known, return it */
    if (n < next_compute) return cache[n];

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = next_compute; i <= n; ++i) {
        idx_t idx = i;

        /* coefficients for the linear recurrence */
        idx_t coeff1 = 2 * (3 * idx - 1);          /* 2*(3n-1)   */
        idx_t coeff2 = idx + 3;                   /* n+3        */
        idx_t coeff3 = 4 * (4 * idx - 13);        /* 4*(4n-13)  */
        idx_t coeff4 = 12 * (idx - 4);            /* 12*(n-4)   */

        /* compute the numerator of the recurrence */
        seq_t term1 = cache[idx - 1] * (seq_t)coeff1;
        seq_t term2 = cache[idx - 2] * (seq_t)coeff2;
        seq_t term3 = cache[idx - 3] * (seq_t)coeff3;
        seq_t term4 = cache[idx - 4] * (seq_t)coeff4;

        seq_t numerator = term1 - term2 - term3 + term4;
        seq_t denominator = (seq_t)(idx + 1);

        cache[i] = numerator / denominator;
    }

    next_compute = n + 1;
    return cache[n];
}
