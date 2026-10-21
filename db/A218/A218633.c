/*
 * A218633  Hilltop maps: number of nX3 binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal, diagonal
 *          or antidiagonal neighbor in a random 0..3 nX3 array.
 * https://oeis.org/A218633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            7*a(n-1)+7*a(n-2)+7*a(n-3)+7*a(n-4)+7*a(n-5)+3*a(n-6)+21*a(n-7)+
 *            6*a(n-8)+6*a(n-9)+6*a(n-10)+6*a(n-11)+3*a(n-12)+9*a(n-13)-
 *            9*a(n-14)-9*a(n-15)-9*a(n-16)-9*a(n-17)-9*a(n-18)-9*a(n-19), for
 *            n>19, with initial terms a(1)=7, a(2)=63, a(3)=511, a(4)=4087,
 *            a(5)=32689, a(6)=261505, a(7)=2092027,...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[19] = {
    7ULL,
    63ULL,
    511ULL,
    4087ULL,
    32689ULL,
    261505ULL,
    2092027ULL,
    16736081ULL,
    133887639ULL,
    1071093011ULL,
    8568679227ULL,
    68548914887ULL,
    548387167731ULL,
    4387064131261ULL,
    35096247367295ULL,
    280767853492265ULL,
    2246125824498091ULL,
    17968870569494163ULL,
    143749876352266497ULL
};

static const int coeffs[19] = {
    7,  7,  7,  7,  7,
    3, 21,  6,  6,  6,
    6,  3,  9,
   -9, -9, -9, -9, -9, -9
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    if (n == 0) return (seq_t)0;

    size_t idx = (size_t)(n - 1);

    /* Initialize cache with the first 19 terms if needed */
    if (cache_len == 0) {
        size_t need = 19;
        while (cache_cap < need)
            cache_cap = cache_cap ? cache_cap << 1 : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 19; ++i) cache[i] = init_values[i];
        cache_len = 19;
    }

    /* If the requested term is already cached, return it */
    if (idx < cache_len) return cache[idx];

    /* Generate terms up to n using the recurrence */
    for (size_t cur = cache_len + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        for (int i = 0; i < 19; ++i) {
            seq_t val = cache[cur - 2 - i]; /* a(cur-1-i) */
            if (coeffs[i] >= 0)
                sum += (seq_t)coeffs[i] * val;
            else
                sum -= (seq_t)(-coeffs[i]) * val;
        }

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap << 1 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = sum;
    }

    return cache[idx];
}
