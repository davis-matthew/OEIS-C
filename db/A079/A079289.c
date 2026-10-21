/*
 * A079289  For even n, a(n) = a(n-2) + a(n-1) + 2^(n/2-2), n>2. For odd n,
 *          a(n) = a(n-2) + a(n-1).
 * https://oeis.org/A079289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For even n (n > 2): a(n) = a(n-2) + a(n-1) + 2^(n/2-2) For odd n
 *            (n ≥ 1): a(n) = a(n-2) + a(n-1) Base values: a(0) = 1, a(1) = 1.
 *            The function is memoised; calls in any order are supported
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Return the n-th term of OEIS A079289. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms cached (indices 0 .. cache_len-1) */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* enlarge cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();   /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 0;
        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = 1;
        } else {
            seq_t a_nm1 = cache[i - 1];
            seq_t a_nm2 = cache[i - 2];
            val = a_nm1 + a_nm2;
            if ((i & 1) == 0 && i > 2) {          /* even index > 2 */
                idx_t exp = ((idx_t)(i >> 1)) - 2;
                seq_t add = pow2(exp);             /* 2^(i/2-2) */
                val += add;
            }
        }
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
