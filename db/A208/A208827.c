/*
 * A208827  Number of 6-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 77, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208827
   Number of 6‑bead necklaces labeled with numbers –n..n allowing reversal,
   with sum zero.

   Recurrence (n > 6):
      a(n) = 6*a(n-1) – 15*a(n-2) + 20*a(n-3) – 15*a(n-4) + 6*a(n-5) – a(n-6)

   Initial values:
      a(1) = 18,  a(2) = 167,  a(3) = 828,  a(4) = 2821,
      a(5) = 7582, a(6) = 17339
*/

#include <stdlib.h>

extern seq_t a(idx_t n);   /* forward declaration for C++ compatibility */

seq_t a(idx_t n)
{
    /* On first call, cache will be NULL.  The array is indexed by the
       OEIS offset: index 1 contains a(1).  cache[0] is unused.      */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (bytes count) */
    static idx_t cache_max = 0;    /* highest computed index */

    /* Ensure the cache is large enough to hold up to index n.          */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n) + 1;   /* +1 because index 0 is unused */
        if (new_cap < 8) new_cap = 8;       /* reserve some spare space   */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* If the requested value is already computed, return it.            */
    if (n <= cache_max)
        return cache[n];

    /* Initial values for n = 1 … 6.                                      */
    static const seq_t init[7] = {
        0,          /* unused, offset 1 */
        18, 167, 828, 2821, 7582, 17339
    };

    /* Compute all values from cache_max + 1 up to n.                      */
    idx_t i;
    for (i = cache_max + 1; i <= n; ++i) {
        if (i <= 6) {                     /* use the initial table */
            cache[i] = init[i];
        } else {                          /* apply the recurrence   */
            seq_t pos = cache[i-1] * (seq_t)6
                       + cache[i-3] * (seq_t)20
                       + cache[i-5] * (seq_t)6;
            seq_t neg = cache[i-2] * (seq_t)15
                       + cache[i-4] * (seq_t)15
                       + cache[i-6];
            cache[i] = pos - neg;
        }
    }
    cache_max = n;

    return cache[n];
}
