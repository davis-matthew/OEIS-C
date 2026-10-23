/*
 * A049778  a(n) = Sum_{k=1..floor((n+1)/2)} T(n,2k-1), array T as in A049777.
 * https://oeis.org/A049778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n*ceil(n/2), a(1)=1
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_comp = 0;

    if (n == 0) return 0;  /* offset is 1 */

    if (max_comp < n) {
        /* Allocate (or reallocate) space up to n */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure – unlikely in tests */

        /* Preserve previously computed values */
        if (cache) {
            memcpy(new_cache, cache,
                   (size_t)(max_comp + 1) * sizeof(seq_t));
            free(cache);
        }

        cache = new_cache;

        /* Initialisation */
        if (max_comp == 0) {
            cache[0] = 0;          /* unused, for 1‑based indexing */
            cache[1] = 1;
            max_comp = 1;
        }

        /* Compute up to n */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t ceil_half = ((seq_t)i + 1) / 2; /* ceil(i/2) */
            seq_t prod = (seq_t)i * ceil_half;
            cache[i] = cache[i - 1] + prod;
        }
        max_comp = n;
    }
    return cache[n];
}
