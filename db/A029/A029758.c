/*
 * A029758  Number of AVL trees of height n.
 * https://oeis.org/A029758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 1, a(n+1) = a(n)^2 + 2*a(n)*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed values */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0;   /* number of elements stored (cache_cache_size-1 == last index) */

/* ensure cache contains terms up to index n */
static void seq_ensure_size(idx_t n)
{
    if (seq_cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = reinterpret_cast<seq_t*>(realloc(seq_cache, new_size * sizeof(seq_t)));
#else
        tmp = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
#endif
        if (!tmp) return;              /* allocation failed – leave cache unchanged */
        seq_cache = tmp;

        /* initialise first terms if cache was empty */
        if (seq_cache_size == 0) {
            seq_cache[0] = (seq_t)1;
            if (new_size > 1)
                seq_cache[1] = (seq_t)1;
        } else if (seq_cache_size == 1 && new_size > 1) {
            seq_cache[1] = (seq_t)1;
        }

        /* compute any missing values */
        for (idx_t i = seq_cache_size; i < new_size; ++i) {
            if (i < 2) continue;  /* already initialised */
            seq_t a_nm1 = seq_cache[i-1];
            seq_t a_nm2 = seq_cache[i-2];
            seq_cache[i] = a_nm1 * a_nm1 + (seq_t)2 * a_nm1 * a_nm2;
        }

        seq_cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    seq_ensure_size(n);
    return seq_cache[n];
}
