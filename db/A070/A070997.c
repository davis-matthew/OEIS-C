/*
 * A070997  a(n) = 8*a(n-1) - a(n-2), a(0)=1, a(-1)=1.
 * https://oeis.org/A070997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) – a(n-2), a(0)=1, a(-1)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_CACHE_N 20001   /* sufficient for the expected n  (≈10 000) */

static seq_t cache[MAX_CACHE_N];
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise first value */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
        if (n == 0) return cache[0];
    }

    /* initialise second value */
    if (cache_len == 1) {
        cache[1] = (seq_t)7;   /* a(1) = 8*a(0) – a(-1) = 8*1 – 1 = 7 */
        cache_len = 2;
    }

    /* already computed? */
    if (n < cache_len) return cache[n];

    /* compute from the first missing term up to n */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = (cache[i-1] * (seq_t)8) - cache[i-2];

    cache_len = n + 1;
    return cache[n];
}
