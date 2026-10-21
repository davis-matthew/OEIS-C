/*
 * A234414  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having the absolute values of all six edge and diagonal
 *          differences no larger than 1.
 * https://oeis.org/A234414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 54*a(n-2) + 57*a(n-3) + 90*a(n-4) - 96*a(n-5)
 *            - 108*a(n-6) - 24*a(n-7) Initial values (n = 1..7): a(1)=106,
 *            a(2)=550, a(3)=2848, a(4)=14794, a(5)=76996, a(6)=401332,
 *            a(7)=2094334
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Multiply a seq_t by a small unsigned integer k.  For built-in
   integral seq_t types up to 64 bits we use an intermediate
   unsigned __int128 to avoid overflow.  For larger or arbitrary
   precision types the ordinary multiplication is used (which
   must support big integers). */
static seq_t mul_small_uint(seq_t v, unsigned int k)
{
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 t = (unsigned __int128)v * k;
        return (seq_t)t;
    } else {
        return v * k;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined, but guard */

    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0, sz = 0;

    /* initial terms (index 1..7) stored – array index is n-1 */
    const seq_t init[7] = {106ULL, 550ULL, 2848ULL, 14794ULL,
                           76996ULL, 401332ULL, 2094334ULL};

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 8 > 7 ? 8 : 7;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        sz = 7;
    }

    if ((idx_t)sz >= n) return cache[n-1];

    /* grow cache if needed */
    if ((size_t)n > cap) {
        while (cap < (size_t)n) cap *= 2;
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    for (idx_t curr = sz + 1; curr <= n; ++curr) {
        size_t i = curr - 1; /* zero‑based index for a(curr) */

        seq_t pos, neg, val;

        /* use 128‑bit intermediates when seq_t is small */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 p1 = (unsigned __int128)cache[i-1] * 13;
            unsigned __int128 p2 = (unsigned __int128)cache[i-3] * 57;
            unsigned __int128 p3 = (unsigned __int128)cache[i-4] * 90;
            unsigned __int128 positive = p1 + p2 + p3;

            unsigned __int128 n1 = (unsigned __int128)cache[i-2] * 54;
            unsigned __int128 n2 = (unsigned __int128)cache[i-5] * 96;
            unsigned __int128 n3 = (unsigned __int128)cache[i-6] * 108;
            unsigned __int128 n4 = (unsigned __int128)cache[i-7] * 24;
            unsigned __int128 negative = n1 + n2 + n3 + n4;

            val = (seq_t)(positive - negative);
        } else {
            pos = mul_small_uint(cache[i-1], 13);
            pos += mul_small_uint(cache[i-3], 57);
            pos += mul_small_uint(cache[i-4], 90);

            neg = mul_small_uint(cache[i-2], 54);
            neg += mul_small_uint(cache[i-5], 96);
            neg += mul_small_uint(cache[i-6], 108);
            neg += mul_small_uint(cache[i-7], 24);

            val = pos - neg;
        }
        cache[i] = val;
    }
    sz = (size_t)n;
    return cache[n-1];
}
