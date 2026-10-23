/*
 * A195657  Number of ways to place 8n nonattacking kings on a vertical
 *          cylinder 16 X 2n.
 * https://oeis.org/A195657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-9) - 32*a(n-8) + 113*a(n-7) - 231*a(n-6)
 *              + 301*a(n-5) - 259*a(n-4) + 147*a(n-3) - 53*a(n-2) + 11*a(n-1)
 * Initial values: a(1)=512, a(2)=17536, a(3)=218052, a(4)=1599820,
 *                 a(5)=8500668, a(6)=36383284, a(7)=133538996,
 *                 a(8)=437500380, a(9)=1314748124
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array indexed by n, 1‑based */
static idx_t cache_limit = 0;    /* highest n that has been cached */

/* ensure that values up to n are computed and cached */
static void ensure_cache(idx_t n)
{
    if (n <= cache_limit)
        return;

    /* grow the cache array */
    size_t new_size = (size_t)(n + 1);  /* space for indices 0..n */
    if (cache == NULL)
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    if (!cache)
        exit(EXIT_FAILURE);          /* allocation failure */

    /* integer constants for the first nine values */
    static const unsigned int init_vals[9] = {
        512U, 17536U, 218052U, 1599820U, 8500668U,
        36383284U, 133538996U, 437500380U, 1314748124U
    };

    /* compute values from cache_limit+1 up to n */
    for (idx_t i = cache_limit + 1; i <= n; ++i) {
        if (i <= 9) {           /* initial values */
            cache[i] = (seq_t)init_vals[i - 1];
            continue;
        }

        /* recurrence for i > 9 */
        seq_t val = (seq_t)0;
        val += cache[i - 9] * (seq_t)4;
        val -= cache[i - 8] * (seq_t)32;
        val += cache[i - 7] * (seq_t)113;
        val -= cache[i - 6] * (seq_t)231;
        val += cache[i - 5] * (seq_t)301;
        val -= cache[i - 4] * (seq_t)259;
        val += cache[i - 3] * (seq_t)147;
        val -= cache[i - 2] * (seq_t)53;
        val += cache[i - 1] * (seq_t)11;
        cache[i] = val;
    }

    cache_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* below the official offset, value is irrelevant */
    ensure_cache(n);
    return cache[n];
}
