/*
 * A050067  a(n)=a(n-1)+a(m), where m=2n-2-2^(p+1) and 2^p<n-1<=2^(p+1), for n
 *          >= 4.
 * https://oeis.org/A050067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=3; for n>=4, a(n)=a(n-1)+a(m) where
 *            m=2n-2-2^(p+1) and 2^p < n-1 <= 2^(p+1)
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 125, u64 n <= 503,
 *            u128 n <= 3079, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;       /* dynamic array of sequence values */
static idx_t cache_size = 0;          /* allocated capacity */
static idx_t max_computed = 0;        /* highest n computed so far */

/* floor(log2(v)) for v>0 */
static inline idx_t log2_floor(idx_t v)
{
    idx_t p = 0;
    while (v >>= 1)
        ++p;
    return p;
}

/* ensure cache can hold up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n + 1)
        return;
    idx_t new_size = cache_size ? cache_size << 1 : 1;
    while (new_size < n + 1)
        new_size <<= 1;
    seq_t *new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_arr)        /* allocation failure: fallback to zero */
        return;
    if (seq_cache)
    {
        memcpy(new_arr, seq_cache, sizeof(seq_t) * cache_size);
        free(seq_cache);
    }
    seq_cache = new_arr;
    cache_size = new_size;
}

/* initialize base values (only once) */
static void init_base(void)
{
    if (max_computed)
        return;
    ensure_cache(3);
    seq_cache[1] = 1;    /* a(1) */
    seq_cache[2] = 3;    /* a(2) */
    seq_cache[3] = 3;    /* a(3) */
    max_computed = 3;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    init_base();

    if (n > max_computed)
    {
        ensure_cache(n);
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            idx_t t = i - 1;
            idx_t p = log2_floor(t - 1);                /* 2^p < t <= 2^(p+1) */
            idx_t pow2 = ((idx_t)1) << (p + 1);        /* 2^(p+1) */
            idx_t m = (i << 1) - 2 - pow2;             /* 2*i - 2 - pow2 */
            seq_cache[i] = seq_cache[i - 1] + seq_cache[m];
        }
        max_computed = n;
    }

    return seq_cache[n];
}
