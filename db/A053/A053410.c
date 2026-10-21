/*
 * A053410  a(1) = 0, a(2) = 16, a(2n+1) = 10*a(2n) - a(2n-1), a(2n) =
 *          10*a(2n-1) - a(2n-2) + 16.
 * https://oeis.org/A053410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A053410.
   Recurrence:
     a(1) = 0,
     a(2) = 16,
     a(2n)   = 10*a(2n-1) - a(2n-2) + 16,
     a(2n+1) = 10*a(2n)   - a(2n-1).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms, index 0 unused */
    static size_t computed = 0;   /* highest index that has been computed */

    if (n == 0) return (seq_t)0;   /* defensive, sequence is defined from n=1 */

    if (n > computed) {
        /* Grow the cache to at least size n+1 (index 0 unused) */
        size_t new_size = n + 1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Compute missing terms from computed+1 up to n */
        for (size_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)0;
            } else if (i == 2) {
                cache[2] = (seq_t)16;
            } else {
                seq_t prev  = cache[i - 1];
                seq_t prev2 = cache[i - 2];
                if (i & 1) {                     /* odd index */
                    cache[i] = (seq_t)10 * prev - prev2;
                } else {                         /* even index */
                    cache[i] = (seq_t)10 * prev - prev2 + (seq_t)16;
                }
            }
        }
        computed = n;
    }
    return cache[n];
}
