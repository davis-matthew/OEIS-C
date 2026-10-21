/*
 * A238147  The number of P-positions in the game of Nim with up to five
 *          piles, allowing for piles of zero, such that the total number of
 *          objects in all piles doesn't exceed 2n.
 * https://oeis.org/A238147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=11, a(2)=26
 * Recurrence:
 *   for n≥1  a(2n+1) = 11·a(n) + 5·a(n-1)   (odd indices)
 *   for n≥0  a(2n+2) = a(n+1) + 15·a(n)   (even indices)
 */
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache of already‑computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Expand cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_len);
        if (!new_cache) abort();          /* allocation failed */

        /* Copy old values if any */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Free old cache */
        if (cache) free_seq_array(cache);

        cache = new_cache;
        cache_len = new_len;

        /* Compute new values */
        for (size_t i = 0; i < cache_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)11;
            } else if (i == 2) {
                cache[i] = (seq_t)26;
            } else if (i & 1) { /* odd: 2n+1 */
                idx_t k = (i - 1) / 2;
                seq_t a_k   = cache[(size_t)k];
                seq_t a_km1 = cache[(size_t)(k - 1)];
                cache[i] = (seq_t)11 * a_k + (seq_t)5 * a_km1;
            } else { /* even: 2n+2 */
                idx_t k = (i - 2) / 2;
                seq_t a_k   = cache[(size_t)k];
                seq_t a_kp1 = cache[(size_t)(k + 1)];
                cache[i] = a_kp1 + (seq_t)15 * a_k;
            }
        }
    }
    return cache[(size_t)n];
}
