/*
 * A186582  Number of (n+3) X 5 0..2 arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A186582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1)=8824, a(2)=827, a(3)=1991, a(4)=12263, a(5)=42313,
 *            a(6)=79015, * a(7)=98893, a(8)=104983, a(9)=108805,
 *            a(10)=112735, a(11)=118765, * a(12)=143799, a(13)=230021,
 *            a(14)=395695. * For n>14: * a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4)
 *            - 4*a(n-5) + 2*a(n-6) * + 2*a(n-8) - 2*a(n-9)
 * Range:     u8 none, u16 n <= 5, u32 n <= 41, u64 n <= 107, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ----- internal cache -------------------------------------------- */

static seq_t *cache = NULL;      /* 1‑based cache   */
static idx_t cache_size = 0;     /* maximum index computed (>=1) */

/* initialise cache with the 14 known values */
static void init_cache(void)
{
    cache_size = 14;
    cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
    cache[0] = 0;      /* not used, keeps 1‑based indexing */
    cache[1] = 8824;
    cache[2] = 827;
    cache[3] = 1991;
    cache[4] = 12263;
    cache[5] = 42313;
    cache[6] = 79015;
    cache[7] = 98893;
    cache[8] = 104983;
    cache[9] = 108805;
    cache[10] = 112735;
    cache[11] = 118765;
    cache[12] = 143799;
    cache[13] = 230021;
    cache[14] = 395695;
}

/* ----- sequence routine ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)                      /* undefined for n<1 */
        return 0;

    if (!cache)                     /* first call: initialise */
        init_cache();

    if (n <= cache_size)            /* already known */
        return cache[n];

    /* enlarge cache to hold up to index n */
    size_t new_cap = (size_t)n + 1;  /* +1 to keep 1‑based indices */
    seq_t *c = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!c)                         /* allocation failed */
        return 0;
    cache = c;

    /* compute all missing terms using the empirical recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* positive part: 2*a(i-1) + 2*a(i-4) + 2*a(i-6) + 2*a(i-8) */
        seq_t pos = cache[i-1];
        pos += cache[i-1];           /* *2 */
        pos += cache[i-4];
        pos += cache[i-4];           /* *2 */
        pos += cache[i-6];
        pos += cache[i-6];           /* *2 */
        pos += cache[i-8];
        pos += cache[i-8];           /* *2 */

        /* negative part: a(i-2) + 4*a(i-5) + 2*a(i-9) */
        seq_t neg = cache[i-2];
        seq_t four_a5 = cache[i-5];
        four_a5 += four_a5;           /* *2 */
        four_a5 += four_a5;           /* *4 */
        neg += four_a5;
        seq_t two_a9 = cache[i-9];
        two_a9 += two_a9;             /* *2 */
        neg += two_a9;

        cache[i] = pos - neg;         /* pos ≥ neg here, so no underflow */
    }

    cache_size = n;
    return cache[n];
}
