/*
 * A064562  Reciprocal of n terminates with an infinite repetition of digit 3.
 *          Multiples of 10 are omitted.
 * https://oeis.org/A064562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th smallest number of the form 3·4^i or 3·25^i (i
 *            ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    /* cache of previously generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* current next values from each exponential sequence */
    static seq_t val4  = (seq_t)3;   /* 3·4^i */
    static seq_t val25 = (seq_t)3;   /* 3·25^i */

    size_t target = (size_t)n;

    /* grow cache if necessary */
    if (target > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!tmp && target > 0) return 0;   /* allocation failure */
        cache = tmp;

        /* generate additional terms in ascending order */
        while (cache_len < target) {
            if (val4 < val25) {
                cache[cache_len++] = val4;
                val4 = mul_small(val4, 4);
            } else if (val25 < val4) {
                cache[cache_len++] = val25;
                val25 = mul_small(val25, 25);
            } else { /* equal case – only occurs for the initial 3 */
                cache[cache_len++] = val4;
                val4  = mul_small(val4,  4);
                val25 = mul_small(val25, 25);
            }
        }
    }

    return cache[(size_t)n - 1];
}
