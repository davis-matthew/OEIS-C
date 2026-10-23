/*
 * A031564  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 66.
 * https://oeis.org/A031564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k>1 such that the continued fraction for
 *            * sqrt(k) has an even period and, if the last term of the
 *            periodic part is * deleted, the central term of the remaining
 *            sequence equals 66
 * Range:     u8 none, u16 n <= 278, u32 n <= 3313, u64 n <= 3313,
 *            u128 n <= 3313, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* integer square‑root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* check if k satisfies the defining property */
static inline bool satisfies(idx_t k)
{
    idx_t a0 = isqrt_u64(k);
    if (a0 * a0 == k)
        return false;                   /* perfect square */

    idx_t m = 0, d = 1, a = a0;
    /* dynamic array to store the periodic terms (type idx_t) */
    idx_t *terms = NULL;
    size_t cap   = 0;
    size_t len   = 0;

    while (1) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;

        if (len == cap) {                       /* grow array */
            size_t new_cap = (cap == 0) ? 4 : cap << 1;
            idx_t *p = (idx_t *)realloc(terms,
                                         new_cap * sizeof(idx_t));
            if (!p) abort();                    /* memory failure */
            terms = p;
            cap   = new_cap;
        }
        terms[len++] = a;

        if (a == 2 * a0)                     /* period finished */
            break;
    }

    bool ok = false;
    if ((len & 1) == 0) {                  /* even period */
        size_t mid = len / 2 - 1;           /* central term after deletion */
        ok = (terms[mid] == 66);
    }

    free(terms);
    return ok;
}

/* dynamic cache for the sequence values */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t next_k      = 2;           /* start searching from 2 */

/* grow the cache array */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_ptr;
#ifdef __cplusplus
    new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        new_ptr[i] = cache[i];
    delete[] cache;
#else
    new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    memcpy(new_ptr, cache, cache_len * sizeof(seq_t));
    free(cache);
#endif
    cache      = new_ptr;
    cache_cap  = new_cap;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined below offset, return 0 */
        return 0;

    while (cache_len < n) {
        while (!satisfies(next_k))
            ++next_k;

        grow_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
