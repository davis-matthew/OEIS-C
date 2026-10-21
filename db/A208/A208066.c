/*
 * A208066  Number of n X 5 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 207, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 6*a(n-5)
   - 2*a(n-6) - 2*a(n-7) + a(n-8) for n>9
   Base values:
   a(1)=16, a(2)=256, a(3)=704, a(4)=1936, a(5)=3696,
   a(6)=7056, a(7)=11424, a(8)=18496, a(9)=27200. */
#include <stdlib.h>

static const seq_t BASE[9] = {
    16, 256, 704, 1936, 3696, 7056, 11424, 18496, 27200
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe */
    if (n <= 9) return BASE[n-1];

    if (cache == NULL || n > cache_size) {
        idx_t new_size = n + 1;     /* index 1..n used */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure fallback */
        cache = tmp;
        if (cache_size < 9) {
            for (idx_t i = 1; i <= 9; ++i) cache[i] = BASE[i-1];
        }
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 9) continue;    /* already set */
            seq_t val = 2*cache[i-1];
            val += 2*cache[i-2];
            val -= 6*cache[i-3];
            val += 6*cache[i-5];
            val -= 2*cache[i-6];
            val -= 2*cache[i-7];
            val += cache[i-8];
            cache[i] = val;
        }
        cache_size = n;
    }
    return cache[n];
}
