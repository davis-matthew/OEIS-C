/*
 * A009954  Coordination sequence for FeS2-Marcasite, S position.
 * https://oeis.org/A009954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) + a(n-3) + 3*a(n-4) + 2*a(n-5) * - 2*a(n-7) -
 *            3*a(n-8) - a(n-9) + a(n-11) + a(n-12) * Initial values (OEIS
 *            offset 0): * a(0)=1, a(1)=4, a(2)=17, a(3)=34, a(4)=64, a(5)=93,
 *            * a(6)=153, a(7)=181, a(8)=262, a(9)=298, a(10)=422, *
 *            a(11)=445, a(12)=593, a(13)=621, a(14)=824 * Recurrence...
 * Range:     u8 n <= 7, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised sequence values */
    static idx_t cache_len = 0;      /* current allocated size (indices 0..len-1) */

    /* initialise base values (indices 0..14) exactly once */
    if (!cache) {
        cache_len = 15;                 /* store indices 0..14 */
        cache     = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) {                   /* allocation failed: abort */
            abort();
        }
        cache[0]  = 1;
        cache[1]  = 4;
        cache[2]  = 17;
        cache[3]  = 34;
        cache[4]  = 64;
        cache[5]  = 93;
        cache[6]  = 153;
        cache[7]  = 181;
        cache[8]  = 262;
        cache[9]  = 298;
        cache[10] = 422;
        cache[11] = 445;
        cache[12] = 593;
        cache[13] = 621;
        cache[14] = 824;
    }

    /* return stored value if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache to hold index n (may reallocate) */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {                      /* allocation failed: abort */
        abort();
    }
    cache = tmp;

    /* compute new values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[i-3];
        val += 3 * cache[i-4];
        val += 2 * cache[i-5];
        val += cache[i-11];
        val += cache[i-12];
        val -= cache[i-1];
        val -= 2 * cache[i-7];
        val -= 3 * cache[i-8];
        val -= cache[i-9];
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
