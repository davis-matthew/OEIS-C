/*
 * A083907  Duplicate of A034386.
 * https://oeis.org/A083907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*a(n-1) if GCD(n,a(n-1)) = 1 else a(n) = a(n-1) a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 28, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t cached_max = 0;

    if (n < 1)
        return 0;                     /* offset is 1; behaviour below is irrelevant */

    /* initialise cache with a(1) = 1 */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;                       /* allocation failure */
        cache[0] = 0;                       /* unused */
        cache[1] = (seq_t)1;
        cached_max = 1;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1)
            newcap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;                       /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing entries up to n */
    seq_t prod = cache[cached_max];
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (is_prime(i))
            prod = prod * (seq_t)i;         /* multiply by prime */
        cache[i] = prod;
    }
    cached_max = n;

    return cache[n];
}
