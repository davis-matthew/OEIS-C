/*
 * A164894  Base-10 representation of the binary string formed by appending
 *          10, 100, 1000, 10000, ..., etc., to 1.
 * https://oeis.org/A164894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1; a(n)=a(n-1)*2^n + 2^(n-1) for n>1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cached_n = 0;
    static size_t cap = 0;

    if (n == 0) return 0;    /* offset starts at 1 */

    if (cap < (size_t)(n + 1)) {
        size_t new_cap = (cap > 0 ? cap << 1 : 1);
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    if (cached_n == 0) cache[0] = 0;   /* dummy zero for base */

    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t term  = prev * pow2(i);    /* shift left by i bits */
        seq_t block = pow2(i - 1);       /* appended block */
        cache[i] = term + block;
    }
    cached_n = n;
    return cache[n];
}
