/*
 * A050066  a(n) = a(n-1) + a(m) for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A050066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=a(3)=3; for n≥4, a(n)=a(n-1)+a(m) where m =
 *            n-1-2^p, p = ⌊log₂(n-1)⌋
 * Range:     u8 n <= 30, u16 n <= 192, u32 n <= 1679, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* capacity of cache array (in elements) */
static idx_t  cache_last = 0;/* largest index computed for which cache[i] is valid */

/* Return a(n) for any n ≥ 1 */
seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2 || n == 3) return (seq_t)3;

    /* initialise cache for the first few terms if needed */
    if (cache_last < 3) {
        if (cache_cap < 4) {
            size_t new_cap = 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* crash‑safe fallback */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)3;
        cache_last = 3;
    }

    /* grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* crash‑safe fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* p = floor(log₂(i-1))  →  floor(log₂(i-2)) for i≥4 */
        idx_t temp = i - 2;
        idx_t p = 0;
        while (temp > 1) {
            temp >>= 1;
            ++p;
        }
        /* 2^p as seq_t */
        seq_t power = ((seq_t)1) << (unsigned)p;
        /* convert to idx_t for subtraction (value fits in idx_t) */
        idx_t power_idx = (idx_t)power;
        idx_t m = i - 1 - power_idx;
        cache[(size_t)i] = cache[(size_t)(i - 1)] + cache[(size_t)m];
    }
    cache_last = n;
    return cache[(size_t)n];
}
