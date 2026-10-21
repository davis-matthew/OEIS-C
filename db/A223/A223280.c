/*
 * A223280  Rolling icosahedron face footprints: number of nX6 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves across an icosahedral edge.
 * https://oeis.org/A223280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 63*a(n-1) - 882*a(n-2) + 792*a(n-3) + 35736*a(n-4)
          - 70768*a(n-5) - 246208*a(n-6) + 327936*a(n-7)
          + 146432*a(n-8) - 180224*a(n-9)
   initial values: a(1)=243, a(2)=6831, a(3)=261819, a(4)=10979127,
                   a(5)=473368227, a(6)=20570223999,
                   a(7)=895927195659, a(8)=39047604482055,
                   a(9)=1702160040384051
*/

#include <stdlib.h>

static const int32_t coeff[9] = {
    63, -882, 792, 35736, -70768, -246208, 327936, 146432, -180224
};

static const uint64_t init_val[10] = {
    0ULL,                                    // dummy for index 0
    243ULL, 6831ULL, 261819ULL, 10979127ULL,
    473368227ULL, 20570223999ULL, 895927195659ULL,
    39047604482055ULL, 1702160040384051ULL
};

static seq_t *cache = NULL;
static idx_t  cache_size = 0;
static idx_t  current_len = 0;

static void ensure_cache_size(idx_t n)
{
    if (cache_size >= n + 1)
        return;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* initialise cache for first 9 terms */
    if (current_len < 9) {
        if (cache_size < 10) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * 10);
            cache_size = 10;
        }
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = (seq_t)init_val[i];
        current_len = 9;
    }

    if (n <= 9)
        return cache[n];

    ensure_cache_size(n);

    for (idx_t i = current_len + 1; i <= n; ++i) {
        seq_t res = 0;

        /* positive coefficients first to avoid underflow */
        for (int j = 0; j < 9; ++j) {
            int32_t c = coeff[j];
            if (c >= 0)
                res += (seq_t)c * cache[i - (j + 1)];
        }
        for (int j = 0; j < 9; ++j) {
            int32_t c = coeff[j];
            if (c < 0)
                res -= (seq_t)(-c) * cache[i - (j + 1)];
        }
        cache[i] = res;
    }
    current_len = n;
    return cache[n];
}
