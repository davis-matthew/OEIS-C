/*
 * A233685  Number of (n+1)X(2+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 10, and no
 *          two adjacent values equal.
 * https://oeis.org/A233685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +28*a(n-2) - 25*a(n-3) -103*a(n-4) * + 97*a(n-5) +
 *            68*a(n-6) - 47*a(n-7) -12*a(n-8) + 2*a(n-9) * initial values:
 *            a(1)=108, a(2)=548, a(3)=2628, a(4)=13324, a(5)=64756, *
 *            a(6)=327588, a(7)=1596200, a(8)=8064200, a(9)=39342364
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* Cache of computed terms, indexed from 0. */
static idx_t  cache_cap = 0;      /* Current capacity of the cache. */

static void ensure_cache(idx_t n)
{
    /* Ensure the cache can index up to n (inclusive). */
    if (cache_cap == 0) {
        /* Initial allocation: at least n+1, but also enough for the first 9 terms. */
        cache_cap = (n + 1) > 10 ? n + 1 : 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* Base values for indices 1..9 as given in the OEIS entry. */
        const seq_t base[10] = {
            0ULL,              /* dummy for index 0 */
            108ULL, 548ULL, 2628ULL, 13324ULL, 64756ULL,
            327588ULL, 1596200ULL, 8064200ULL, 39342364ULL
        };
        for (idx_t i = 1; i <= 9 && i < cache_cap; ++i)
            cache[i] = base[i];
    } else if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n = 0; return 0. */

    ensure_cache(n);

    if (n <= 9) return cache[n];

    /* Compute terms from 10 up to n using the recurrence. */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t pos = cache[i - 1]
                    + (seq_t)28 * cache[i - 2]
                    + (seq_t)97 * cache[i - 5]
                    + (seq_t)68 * cache[i - 6]
                    + (seq_t)2  * cache[i - 9];

        seq_t neg = (seq_t)25 * cache[i - 3]
                    + (seq_t)103 * cache[i - 4]
                    + (seq_t)47  * cache[i - 7]
                    + (seq_t)12  * cache[i - 8];

        cache[i] = pos - neg;   /* Guaranteed to be positive for all n. */
    }

    return cache[n];
}
