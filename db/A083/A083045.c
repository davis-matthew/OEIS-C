/*
 * A083045  Main diagonal of table A083044.
 * https://oeis.org/A083045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n, n) where T(n,0) = floor(3n) +1 = 3n+1 and T(n,k+1) =
 *            ceil((3/2) * T(n,k)) which is equivalent to T(n,k+1) = (3 *
 *            T(n,k) + 1) / 2 (integer division)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache previously computed values to avoid recomputation. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow cache to at least n+1 entries. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;          /* Out of memory – caller will get 0. */
        cache = tmp;

        /* Compute all new entries.  */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t val = (seq_t)3 * (seq_t)i + 1;   /* T(i,0) = 3i+1 */
            for (idx_t k = 0; k < i; ++k) {
                /* T(i,k+1) = ceil( (3/2) * val ) = (3*val + 1)/2 */
                val = ((seq_t)3 * val + 1) / 2;
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
