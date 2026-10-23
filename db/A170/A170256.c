/*
 * A170256  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170256.
   a(0) = 1
   For 1 <= n <= 40:  a(n) = 2 + 5 * sum_{k=0}^{n-1} a(k)
   a(41) = 5 * sum_{k=1}^{40} a(k) - 14
   For n >= 42:      a(n) = 15 * a(n-41) - 5 * sum_{k=n-40}^{n-1} a(k)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* Cached values a[0 .. next-1]          */
    static idx_t next = 0;           /* First index not yet computed          */
    static size_t cap = 0;           /* Allocated capacity                     */

    if (n >= next) {
        /* Allocate or grow the cache.  */
        size_t newsize = (size_t)(n + 1);
        if (cap < newsize) {
            cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
            if (!cache) return 0;    /* allocation failed – return 0 */
            cap = newsize;
        }

        /* Compute the sum of all cached values (a[0 .. next-1]) and
           the sum of the last up to 40 values for the recurrence.  */
        seq_t sumAll   = 0;          /* sum of a[0 .. next-1]                */
        seq_t winSum   = 0;          /* sum of a[max(0,next-40) .. next-1]    */
        for (idx_t j = 0; j < next; ++j) sumAll += cache[j];
        size_t winStart = (next > 40 ? next - 40 : 0);
        for (idx_t j = winStart; j < next; ++j) winSum += cache[j];

        for (idx_t i = next; i <= n; ++i) {
            seq_t val = 0;

            if (i == 0) {
                val = (seq_t)1;
            } else if (i <= 40) {
                /* a(i) = 2 + 5 * sum_{k=0}^{i-1} a(k)  */
                val = (seq_t)2 + (seq_t)5 * sumAll;
            } else if (i == 41) {
                /* a(41) = 5 * sum_{k=1}^{40} a(k) - 14  */
                seq_t sumRest = sumAll - cache[0];          /* a1 .. a40 */
                val = (seq_t)5 * sumRest - (seq_t)14;
            } else {
                /* a(i) = 15 * a(i-41) - 5 * windowSum    */
                seq_t term1 = (seq_t)15 * cache[i - 41];
                seq_t term2 = (seq_t)5 * winSum;
                val = term1 - term2;
            }

            cache[i] = val;

            /* Update running sums for the next iterations.  */
            sumAll += val;
            winSum += val;
            if (i >= 40) winSum -= cache[i - 40];
        }

        next = n + 1;
    }

    return cache[n];
}
