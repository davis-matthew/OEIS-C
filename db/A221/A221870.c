/*
 * A221870  Number of nX2 arrays of occupancy after each element moves to some
 *          horizontal, vertical or antidiagonal neighbor, without consecutive
 *          moves in the same direction.
 * https://oeis.org/A221870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   linear recurrence (order 12) * a(n) =
 *            7*a(n-1)+27*a(n-2)-77*a(n-3)-247*a(n-4)+195*a(n-5)+277*a(n-6) *
 *            -51*a(n-7)-32*a(n-8)+103*a(n-9)-78*a(n-10)+25*a(n-11)-4*a(n-12)
 *            * initial values (n ≥ 1): * a(1)=1, a(2)=20, a(3)=162,
 *            a(4)=1442, a(5)=12688, a(6)=110720, * a(7)=970828, a(8)=8489090,
 *            a(9)=743017...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int coeff[12] = {
    7, 27, -77, -247, 195, 277, -51, -32, 103, -78, 25, -4
};

static const seq_t init[12] = {
    (seq_t)1ULL,
    (seq_t)20ULL,
    (seq_t)162ULL,
    (seq_t)1442ULL,
    (seq_t)12688ULL,
    (seq_t)110720ULL,
    (seq_t)970828ULL,
    (seq_t)8489090ULL,
    (seq_t)74301713ULL,
    (seq_t)650080567ULL,
    (seq_t)5688284025ULL,
    (seq_t)49771429169ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t size   = 0;   /* total capacity of cache          */
    static size_t cached = 0;   /* last computed term (1‑based)     */

    /* Allocate initial space and store the 12 initial terms. */
    if (cache == NULL) {
        size = 32;                     /* reasonable start size          */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out‑of‑memory: return 0        */
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cached = 12;
    }

    /* Enlarge cache if it is too small for the requested index. */
    if ((size_t)n > size) {
        size_t new_size = n < size * 2 ? size * 2 : n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;
        cache = tmp;
        size = new_size;
    }

    /* Compute missing terms up to index n. */
    if (n > cached) {
        for (idx_t idx = cached + 1; idx <= n; ++idx) {
            /* idx is 1‑based.  Cache index for a(m) is m‑1. */
#if defined(__cplusplus)
            /* In C++ (big‑int) use unsigned sums and adjust for sign. */
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 12; ++k) {
                seq_t val = cache[(size_t)(idx - 2 - k)];
                int c = coeff[k];
                seq_t prod = val * (seq_t)(c >= 0 ? c : -c);
                if (c >= 0) pos += prod;
                else       neg += prod;
            }
            cache[(size_t)(idx - 1)] = pos - neg;
#else
            /* In C use signed __int128 arithmetic. */
            signed __int128 sum = 0;
            for (int k = 0; k < 12; ++k) {
                seq_t val = cache[(size_t)(idx - 2 - k)];
                int c = coeff[k];
                unsigned int ac = c >= 0 ? (unsigned int)c : (unsigned int)(-c);
                unsigned __int128 prod = (unsigned __int128)val * (unsigned __int128)ac;
                if (c >= 0) sum += (signed __int128)prod;
                else        sum -= (signed __int128)prod;
            }
            cache[(size_t)(idx - 1)] = (seq_t)sum;
#endif
        }
        cached = n;
    }

    return cache[(size_t)(n - 1)];
}
