/*
 * A063218  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          51 ).
 * https://oeis.org/A063218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) with a(1)=3, a(2)=8, a(3)=14,
 *            a(4)=18, a(5)=24
 * Range:     u8 n <= 48, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* current allocation size   */
    static size_t upto = 0;   /* number of elements already computed */

    if (n == 0) return 0; /* sequence is defined for n>=1 only */

    size_t idx = (size_t)(n - 1);   /* zero‑based index */

    /* extend the cache if necessary */
    if (idx + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed – return 0 (undefined) */
        cache = tmp;
        cap = new_cap;
    }

    /* initialise initial terms the first time they are needed */
    if (upto < 5) {
        cache[0] = 3;
        cache[1] = 8;
        cache[2] = 14;
        cache[3] = 18;
        cache[4] = 24;
        upto = 5;
    }

    /* compute further terms up to the required index */
    for (size_t i = upto; i <= idx; ++i) {
        if (i < 5) {            /* safety: initial terms already set */
            continue;
        }
        cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
    }
    upto = idx + 1;

    return cache[idx];
}
