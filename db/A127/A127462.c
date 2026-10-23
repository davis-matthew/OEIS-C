/*
 * A127462  a(1)=1; for n>1, a(n) = number of earlier terms a(k), 1<=k<=n-1,
 *          such that (k+a(k)) is coprime to n.
 * https://oeis.org/A127462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=#{k:1≤k<n | gcd(k+a(k),n)=1} */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of computed values; indices start at 1 */
    static seq_t *cache = NULL;
    static size_t cap   = 0;       /* allocated size (index 0 unused) */
    static idx_t  max_n = 0;       /* largest n already computed */

    if (n == 0)                    /* defensive guard – OEIS offset starts at 1 */
        return 0;

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        /* cast is needed for C++; implicit conversion works in C */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
            continue;
        }
        seq_t count = 0;
        seq_t idx   = (seq_t)i;     /* cast once for speed */
        for (idx_t j = 1; j < i; ++j) {
            seq_t v = (seq_t)j + cache[j];
            if (gcd(v, idx) == 1)
                ++count;
        }
        cache[i] = count;
    }

    max_n = n;
    return cache[n];
}
