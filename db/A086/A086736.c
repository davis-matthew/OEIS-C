/*
 * A086736  Sum(j=1,n,floor(A000041(j)/j)).
 * https://oeis.org/A086736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1}^n floor(p(j)/j) where p(j) is the partition
 *            function. The implementation caches p(j) and the cumulative sums
 *            in dynamic arrays so that repeated calls are answered in O(1)
 *            after the first expansion. It uses only C11 standard features
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  seq_t and idx_t are typedef’d elsewhere in the build. */
seq_t a(idx_t n)
{
    /* cached partition values p(0..maxComputed) */
    static seq_t *part = NULL;
    /* cached cumulative sums a(0..maxComputed) = sum_{j=1}^i floor(p(j)/j) */
    static seq_t *pref = NULL;
    /* number of entries that have been computed so far */
    static idx_t maxComputed = 0;
    /* capacity of the allocated arrays */
    static size_t allocCap = 0;

    /* ensure arrays are large enough for index n */
    if ((size_t)n >= allocCap) {
        size_t newCap = (n < 16 ? 16 : n * 2) + 1;  /* +1 for index 0 */
        /* resize arrays */
        part = (seq_t *)realloc(part, newCap * sizeof(seq_t));
        pref = (seq_t *)realloc(pref, newCap * sizeof(seq_t));
        /* initialise newly allocated elements */
        for (size_t i = allocCap; i < newCap; ++i) {
            part[i] = (seq_t)0;
            pref[i] = (seq_t)0;
        }
        /* if this is the first allocation, set p(0)=1 */
        if (allocCap == 0) {
            part[0] = (seq_t)1;   /* partition of 0 is 1 */
            pref[0] = (seq_t)0;   /* no terms summed yet */
        }
        allocCap = newCap;
    }

    /* compute partition values and the cumulative sums up to n */
    if (n > maxComputed) {
        for (idx_t j = maxComputed + 1; j <= n; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; ; ++k) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > j) break;
                sum += ((k & 1) ? part[j - g1] : -part[j - g1]);

                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 > j) continue;
                sum += ((k & 1) ? part[j - g2] : -part[j - g2]);
            }
            part[j] = sum;

            seq_t t = part[j] / (seq_t) j;   /* floor division */
            pref[j] = pref[j - 1] + t;
        }
        maxComputed = n;
    }

    return pref[n];
}
