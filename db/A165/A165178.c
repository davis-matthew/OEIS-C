/*
 * A165178  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff(n) + 44·(a(n‑1)+…+a(n‑7)) – 990·a(n‑8)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   grow_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where coeff(n) = 1 for n=0 or n=8, 2 for 1≤n≤7, and 0 otherwise. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static idx_t len = 0;    /* highest index computed + 1 */

    /* initialise storage on first call */
    if (cache == NULL) {
        cache = seq_alloc_array(1);
        cap   = 1;
        len   = 0;
    }

    /* ensure the array is large enough for index n */
    if ((size_t)(n + 1) > cap) {
        grow_seq_array(&cache, &cap, n + 1);
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val;
        /* coefficient from the numerator of the generating function */
        if (i == 0 || i == 8) {
            val = 1;
        } else if (i >= 1 && i <= 7) {
            val = 2;
        } else {
            val = 0;
        }

        /* add 44·a(i‑k) for k=1..7 */
        for (idx_t k = 1; k <= 7 && i >= k; ++k) {
            val += (seq_t)44 * cache[i - k];
        }

        /* subtract 990·a(i‑8) if applicable */
        if (i >= 8) {
            val -= (seq_t)990 * cache[i - 8];
        }

        cache[i] = val;
    }

    len = n + 1;
    return cache[n];
}
