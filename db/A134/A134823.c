/*
 * A134823  Expansion of -x*(x^5+11*x^4+x^3+x^2+21*x-4) /
 *          ((x^2+x-1)*(3*x^4+7*x-1)).
 * https://oeis.org/A134823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8*a(n-1) - 6*a(n-2) - 7*a(n-3) + 3*a(n-4) - 3*a(n-5) -
 *            3*a(n-6) offset 1: a(1)=4, a(2)=11, a(3)=63, a(4)=409,
 *            a(5)=2818, a(6)=19669
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n);

/* static cache for memoization */
static seq_t *cache = NULL;   /* array storing a(1)...a(k) at index k-1 */
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */

/* ensure cache has capacity at least n (n >= 1) */
static void ensure_cache(idx_t n)
{
    /* n can be up to 10k, cast safely to size_t */
    size_t required = (size_t)n;
    if (required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < required) new_cap = required;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failure: abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* initialize cache with the first six terms */
static void init_cache(void)
{
    if (cache) return; /* already initialized */
    ensure_cache(16);
    cache[0] = (seq_t)4;
    cache[1] = (seq_t)11;
    cache[2] = (seq_t)63;
    cache[3] = (seq_t)409;
    cache[4] = (seq_t)2818;
    cache[5] = (seq_t)19669;
    cache_size = 6;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined by OEIS, but harmless */
    init_cache();
    if ((size_t)n > cache_cap) ensure_cache(n);
    if ((size_t)n > cache_size)
    {
        size_t i;
        /* compute terms cache_size+1 .. n */
        for (i = cache_size; i < (size_t)n; ++i)
        {
            /* i is zero‑based index of term a(i+1) */
            seq_t val = (8  * cache[i-1]          )
                      - (6  * cache[i-2]          )
                      - (7  * cache[i-3]          )
                      + (3  * cache[i-4]          )
                      - (3  * cache[i-5]          )
                      - (3  * cache[i-6]          );
            cache[i] = val;
        }
        cache_size = (size_t)n;
    }
    return cache[n-1];
}
