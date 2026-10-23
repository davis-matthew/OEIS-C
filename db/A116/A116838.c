/*
 * A116838  Number of permutations of length n which avoid the patterns 1342,
 *          3421, 4312.
 * https://oeis.org/A116838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 72*a(n-2) + 222*a(n-3) - 418*a(n-4) +
 *            496*a(n-5) - 369*a(n-6) + 165*a(n-7) - 40*a(n-8) + 4*a(n-9) for
 *            n ≥ 11, with initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21,
 *            a(5)=72, a(6)=233, a(7)=719, a(8)=2146, a(9)=6260, a(10)=17968
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* initialize the first 10 values on the first call */
    if (!cache) {
        cache = (seq_t *)malloc(11 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 21;
        cache[5] = 72;
        cache[6] = 233;
        cache[7] = 719;
        cache[8] = 2146;
        cache[9] = 6260;
        cache[10] = 17968;
        cache_size = 10;
    }

    if (n <= cache_size)
        return cache[n];

    /* enlarge cache to hold n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0; /* allocation failed */
    cache = new_cache;

    /* compute terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i < 11) {
            /* values up to a(10) are already initialized */
            continue;
        }
        if (sizeof(seq_t) <= 8) { /* 64‑bit or smaller – use 128‑bit temp */
            unsigned __int128 pos = 0, neg = 0;
            pos += (unsigned __int128)cache[i - 1] * 13;
            pos += (unsigned __int128)cache[i - 3] * 222;
            pos += (unsigned __int128)cache[i - 5] * 496;
            pos += (unsigned __int128)cache[i - 7] * 165;
            pos += (unsigned __int128)cache[i - 9] * 4;

            neg += (unsigned __int128)cache[i - 2] * 72;
            neg += (unsigned __int128)cache[i - 4] * 418;
            neg += (unsigned __int128)cache[i - 6] * 369;
            neg += (unsigned __int128)cache[i - 8] * 40;

            cache[i] = (seq_t)(pos - neg);
        } else { /* arbitrary‑precision */
            seq_t pos = cache[i - 1] * 13 + cache[i - 3] * 222 +
                         cache[i - 5] * 496 + cache[i - 7] * 165 +
                         cache[i - 9] * 4;
            seq_t neg = cache[i - 2] * 72 + cache[i - 4] * 418 +
                         cache[i - 6] * 369 + cache[i - 8] * 40;
            cache[i] = pos - neg;
        }
    }

    cache_size = n;
    return cache[n];
}
