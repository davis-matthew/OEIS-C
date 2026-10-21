/*
 * A217951  Number of n element 0..3 arrays with each element the minimum of 5
 *          adjacent elements of a random 0..3 array of n+4 elements.
 * https://oeis.org/A217951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n‑1) - 6*a(n‑2) + 4*a(n‑3) - a(n‑4) + 6*a(n‑6) -
 *            4*a(n‑7) + 6*a(n‑8) + 4*a(n‑9) + 5*a(n‑10) + 6*a(n‑11) +
 *            2*a(n‑12) + 4*a(n‑13) + 3*a(n‑14) + 2*a(n‑15) + a(n‑16) Initial
 *            values: a(1)=4, a(2)=16, a(3)=50, a(4)=130, a(5)=296, a(6)=610,
 *            a(7)=1177, a(8)=2209, a(9)=4202, a(10)=8366, a(11)=1...
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 57, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _seed[16] = {
    4, 16, 50, 130, 296, 610,
    1177, 2209, 4202, 8366, 17569,
    38251, 84134, 183334, 392284, 824110
};

static seq_t *cache = NULL;     /* cached sequence values (zero‑based) */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure the cache has at least 'needed' slots */
static void _ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Public function: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* outside offset */

    /* initialise cache on first call */
    if (cache_len == 0) {
        _ensure_cache(16);
        for (size_t i = 0; i < 16; ++i)
            cache[i] = _seed[i];
        cache_len = 16;
    }

    /* already computed */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* compute missing terms up to n */
    size_t need = (size_t)n;
    _ensure_cache(need);
    for (size_t idx = cache_len; idx < need; ++idx) {
        /* positive contributions */
        seq_t pos =
            ((seq_t)4 * cache[idx - 1]) + ((seq_t)4 * cache[idx - 3]) +
            ((seq_t)6 * cache[idx - 6]) + ((seq_t)6 * cache[idx - 8]) +
            ((seq_t)4 * cache[idx - 9]) + ((seq_t)5 * cache[idx -10]) +
            ((seq_t)6 * cache[idx -11]) + ((seq_t)2 * cache[idx -12]) +
            ((seq_t)4 * cache[idx -13]) + ((seq_t)3 * cache[idx -14]) +
            ((seq_t)2 * cache[idx -15]) +        cache[idx -16];

        /* negative contributions */
        seq_t neg =
            ((seq_t)6 * cache[idx - 2]) + cache[idx - 4] +
            ((seq_t)4 * cache[idx - 7]);

        cache[idx] = pos - neg;   /* pos >= neg for all n >= 17 */
    }
    cache_len = need;
    return cache[n - 1];
}
