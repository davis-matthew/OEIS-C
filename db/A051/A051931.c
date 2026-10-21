/*
 * A051931  Number of independent sets of nodes in graph K_6 X C_n (n > 2).
 * https://oeis.org/A051931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 7*a(n-2) + a(n-3); Initial values: a(0)=7,
 *            a(1)=1, a(2)=43
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < cache_len)
        return seq_cache[n];

    /* Grow cache to hold index n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp)
        return 0;   /* allocation failure (unlikely for n ≤ 10000) */

    seq_cache = tmp;

    /* Compute missing terms */
    for (idx_t i = cache_len; i < new_len; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)7;
        } else if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)43;
        } else {
            val = 5 * seq_cache[i-1] + 7 * seq_cache[i-2] + seq_cache[i-3];
        }
        seq_cache[i] = val;
    }

    cache_len = new_len;
    return seq_cache[n];
}
