/*
 * A124385  For all n >= 2, Sum_{2<=k<=n, gcd(k,n)>1} a(k) = 1. a(1)=1.
 * https://oeis.org/A124385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; for n>1, a(n)=1 - Σ_{2 <= k <= n-1, gcd(k,n)>1} a(k)
 * Range:     i8 n <= 65, i16 n <= 115, i32 n <= 227, i64 n <= 445,
 *            i128 n <= 881, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A124385.
 *
 * The function uses a static cache so that each term is computed at most once,
 * regardless of the order of requested indices.  The recurrence is evaluated
 * directly with integer gcds, which suffices for values up to about 10 000
 * within the time limits.  All arithmetic is performed in seq_t, the
 * sequence value type supplied by the build.
 */
#include <stdlib.h>
#include <stdint.h>

/* Compute gcd on idx_t (uint64_t) values. */
static idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Static cache: cache[1..n] holds computed values.  Zero-initialised. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* largest index that has been computed */

    if (n == 0) {
        /* Undefined for 0, but return 0 for safety */
        return (seq_t)0;
    }

    /* Ensure cache is large enough. */
    if (n > cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failure: terminate program. */
            abort();
        }
        cache = new_cache;

        /* Initialise new slots to 0. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)0;
        }

        /* Compute values from (cache_size + 1) up to n. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
                continue;
            }
            /* For i>=2 compute s = Σ_{2<=k<i, gcd(k,i)>1} a(k) */
            seq_t s = (seq_t)0;
            for (idx_t k = 2; k < i; ++k) {
                if (gcd_uint64(k, i) > 1) {
                    s += cache[k];
                }
            }
            cache[i] = (seq_t)1 - s;
        }

        cache_size = n;
    }

    return cache[n];
}
