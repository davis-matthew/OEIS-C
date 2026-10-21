/*
 * A206232  Number of (n+1)X3 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly two equal edges, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A206232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+20*a(n-2)-14*a(n-3)-133*a(n-4)+95*a(n-5) *
 *            +123*a(n-6)+9*a(n-7)-102*a(n-8) for n>10 * Initial values
 *            a(1)=60, a(2)=256, a(3)=1136, a(4)=5728, a(5)=31652, *
 *            a(6)=170728, a(7)=943584, a(8)=5175034, a(9)=28475596,
 *            a(10)=156707492
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */

    /* static cache of already computed terms (1‑based indexing) */
    static seq_t *cache   = NULL;         /* array with dummy cache[0] */
    static idx_t max_cached = 0;          /* largest index stored */

    /* initialise cache on first call */
    if (!cache) {
        /* store the first 10 terms as initial conditions */
        max_cached = 10;
        cache = (seq_t *)malloc((max_cached + 1) * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)60;
        cache[2] = (seq_t)256;
        cache[3] = (seq_t)1136;
        cache[4] = (seq_t)5728;
        cache[5] = (seq_t)31652;
        cache[6] = (seq_t)170728;
        cache[7] = (seq_t)943584;
        cache[8] = (seq_t)5175034;
        cache[9] = (seq_t)28475596;
        cache[10] = (seq_t)156707492;
    }

    /* if the requested index is already cached, return it */
    if (n <= max_cached)
        return cache[n];

    /* enlarge the cache to at least n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute new terms up to n using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        /* positive part of the recurrence */
        pos += (seq_t)3   * cache[i - 1];
        pos += (seq_t)20  * cache[i - 2];
        pos += (seq_t)95  * cache[i - 5];
        pos += (seq_t)123 * cache[i - 6];
        pos += (seq_t)9   * cache[i - 7];
        /* negative part of the recurrence */
        neg += (seq_t)14   * cache[i - 3];
        neg += (seq_t)133  * cache[i - 4];
        neg += (seq_t)102  * cache[i - 8];
        cache[i] = pos - neg;   /* pos >= neg for all admissible n */
    }

    max_cached = n;
    return cache[n];
}
