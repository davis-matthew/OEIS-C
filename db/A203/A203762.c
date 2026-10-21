/*
 * A203762  a(n)=f(a(n-1),a(n-2)+1,a(n-3)), where f(x,y,z)=yz+zx+xy and
 *          (a(1),a(2),a(3))=(0,0,1).
 * https://oeis.org/A203762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(a(n-1), a(n-2)+1, a(n-3)) with f(x,y,z) = y*z + z*x +
 *            x*y a(1) = 0, a(2) = 0, a(3) = 1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure the cache can store indices up to idx (0‑based). */
static void ensure_cache(idx_t idx)
{
    if (idx < cache_len)
        return;
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap <= idx)
        cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();          /* out of memory */
    cache = tmp;
}

/* Compute the nth term of A203762.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined, but safe return */

    idx_t needed = n - 1;      /* 0‑based index of the term to return */

    /* Initialize base cases if needed */
    if (cache_len == 0) {
        ensure_cache(2);          /* need indices 0,1,2 for a1..a3 */
        cache[0] = (seq_t)0;      /* a1 */
        cache[1] = (seq_t)0;      /* a2 */
        cache[2] = (seq_t)1;      /* a3 */
        cache_len = 3;
    }

    /* If the requested term is already computed, return it */
    if (needed < cache_len)
        return cache[needed];

    /* Compute terms up to the needed index */
    ensure_cache(needed);
    for (idx_t i = cache_len; i <= needed; ++i) {
        seq_t x = cache[i - 1];          /* a(n-1) */
        seq_t y = cache[i - 2] + (seq_t)1;/* a(n-2)+1 */
        seq_t z = cache[i - 3];          /* a(n-3) */
        /* f(x,y,z) = y*z + z*x + x*y */
        cache[i] = y * z + z * x + x * y;
    }
    cache_len = needed + 1;
    return cache[needed];
}
