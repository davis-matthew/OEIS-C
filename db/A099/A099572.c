/*
 * A099572  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k+4, k).
 * https://oeis.org/A099572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/2⌋} binomial(n‑k+4, k) Recurrence (derived
 *            from (1‑x²)⁴(1‑x‑x²)G(x)=1): a(n) =
 *            a(n‑1)+5a(n‑2)-4a(n‑3)-10a(n‑4)+6a(n‑5)+10a(n‑6)-4a(n‑7)-5a(n‑8)+a(n‑9)+a(n‑10)
 *            with initial values a(0)=1, a(1)=1, a(2)=6, a(3)=7, a(4)=23,
 *            a(5)=30, a(6)=73, a(7)=103, a(8)=211, a(9)=314
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* helper for small multiplications (seq_t is an unsigned integer type) */
static inline seq_t mul_small(seq_t v, unsigned int m)
{
    return v * (seq_t)m;
}

/* cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* first ten terms of the sequence */
static const seq_t init_vals[10] =
{
    (seq_t)1,   /* a(0)  */
    (seq_t)1,   /* a(1)  */
    (seq_t)6,   /* a(2)  */
    (seq_t)7,   /* a(3)  */
    (seq_t)23,  /* a(4)  */
    (seq_t)30,  /* a(5)  */
    (seq_t)73,  /* a(6)  */
    (seq_t)103, /* a(7)  */
    (seq_t)211, /* a(8)  */
    (seq_t)314  /* a(9)  */
};

seq_t a(idx_t n)
{
    /* if already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Ensure cache has space for n+1 terms */
    idx_t needed = n + 1;
    if (cache_len == 0) {
        size_t cap = needed > 10 ? needed : 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failure – caller must handle */
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
        cache_len = 10;
        cache_cap = cap;
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;           /* allocation failure – caller must handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 10) {
            cache[i] = init_vals[i];
            continue;
        }
        /* positive contributions */
        seq_t pos = cache[i - 1];
        pos += mul_small(cache[i - 2], 5);
        pos += mul_small(cache[i - 5], 6);
        pos += mul_small(cache[i - 6], 10);
        pos += cache[i - 9];
        pos += cache[i - 10];

        /* negative contributions */
        seq_t neg = mul_small(cache[i - 3], 4);
        neg += mul_small(cache[i - 4], 10);
        neg += mul_small(cache[i - 7], 4);
        neg += mul_small(cache[i - 8], 5);

        cache[i] = pos - neg; /* safe: pos >= neg for all n >= 10 */
    }

    cache_len = n + 1;
    return cache[n];
}
