/*
 * A183136  a(n) = [1/r]+[2/r]+...+[n/r], where r = golden ratio =
 *          (1+sqrt(5))/2 and []=floor.
 * https://oeis.org/A183136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/φ)+floor(2/φ)+…+floor(n/φ), where φ =
 *            (1+sqrt(5))/2 is the golden ratio. We compute each term via
 *            floor(k/φ) = floor((floor(k·sqrt(5)) – k)/2)
 * Range:     u8 n <= 29, u16 n <= 460, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static cache to make calls order‑independent */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* integer square root for 64‑bit values */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= (unsigned __int128)x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (n <= cache_size) return cache[n - 1];

    /* expand cache as needed */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* compute floor(i·sqrt(5)) without floating point   */
        unsigned __int128 prod = (unsigned __int128)i * i * 5;
        idx_t sqrt5i = isqrt_u64((idx_t)prod);       /* safe for n<~10^6 */
        seq_t term = (seq_t)((sqrt5i > i) ? ((sqrt5i - i) / 2) : 0);

        if (i == 1)
            cache[0] = term;                 /* a(1) = term (0) */
        else
            cache[i - 1] = cache[i - 2] + term;
    }
    cache_size = n;
    return cache[n - 1];
}
