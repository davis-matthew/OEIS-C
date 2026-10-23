/*
 * A147646  If A139251 is written as a triangle with rows of lengths 1, 2, 4,
 *          8, 16, ..., the n-th row begins with 2^n followed by the first
 *          2^n-1 terms of the present sequence.
 * https://oeis.org/A147646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4; a(2)=8; for n≥3 let n=2^i+j (0≤j<2^i) if j=0: a(n)=2*n+4
 *            if j=2^i-1: a(n)=2*a(j)+a(j+1)-4 otherwise: a(n)=2*a(j)+a(j+1)
 * Range:     u8 n <= 29, u16 n <= 2039, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> cache;
static void ensure_cache(idx_t n)
{
    if (n >= cache.size())
        cache.resize((size_t)n + 1, (seq_t)0);
}
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_buf)
        abort();
    /* initialize new part to zero */
    memset(new_buf + cache_cap + 1, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}
#endif

/* fast floor(log₂) for unsigned 64‑bit values */
static idx_t floor_log2(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined for n=0 – safety only */

    ensure_cache(n);

#if defined(__cplusplus)
    if (cache[n] != (seq_t)0)
        return cache[n];
#else
    if (cache[n] != (seq_t)0)
        return cache[n];
#endif

    seq_t val;
    if (n == 1) {
        val = (seq_t)4;
    } else if ((n & (n - 1)) == 0) {   /* power of two */
        val = (seq_t)2 * (seq_t)n + (seq_t)4;
    } else {
        idx_t i  = floor_log2(n);
        idx_t pw = ((idx_t)1) << i;     /* 2^i */
        idx_t j  = n - pw;              /* 0 < j < 2^i */
        seq_t a_j   = a(j);
        seq_t a_jp1 = a(j + 1);
        if (j == pw - 1)           /* last element of the block */
            val = (seq_t)2 * a_j + a_jp1 - (seq_t)4;
        else                       /* middle elements */
            val = (seq_t)2 * a_j + a_jp1;
    }

#if defined(__cplusplus)
    cache[n] = val;
#else
    cache[n] = val;
#endif
    return val;
}
