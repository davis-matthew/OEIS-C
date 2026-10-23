/*
 * A199142  Number of n X 1 0..3 arrays with values 0..3 introduced in row
 *          major order and each element equal to one or two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A199142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + a(n-4) + 3*a(n-5) for n ≥
 *            8 Initial values: a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3,
 *            a(6)=6, a(7)=11
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 31, u64 n <= 57, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* 1‑based cache */
static idx_t cache_nmax = 0;
static const seq_t init_vals[8] = {0, 0, 1, 1, 2, 3, 6, 11};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* harmless for below offset */

    if (cache && n <= cache_nmax) return cache[n];

    size_t newsize = (size_t)n + 1; /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) return 0;            /* allocation failed */
    cache = tmp;

    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init_vals[i];
        } else {
            seq_t temp = 3 * cache[i-1];
            temp += cache[i-2];
            temp += cache[i-4];
            temp += 3 * cache[i-5];
            temp -= 7 * cache[i-3];
            cache[i] = temp;
        }
    }

    cache_nmax = n;
    return cache[n];
}
