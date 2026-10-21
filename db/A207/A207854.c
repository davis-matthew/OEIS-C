/*
 * A207854  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) + a(n-2) + 18*a(n-4) + 4*a(n-5)
 *            - 2*a(n-6) - 8*a(n-7) - a(n-8) + a(n-10)
 *
 * Initial values:
 * a(1)=9, a(2)=81, a(3)=289, a(4)=1369, a(5)=5929,
 * a(6)=26569, a(7)=117649, a(8)=522729, a(9)=2319529, a(10)=10297681
 *
 * This implementation memoises the sequence in a static array.
 * It grows the cache as needed and supports calls with n in arbitrary order.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* base values for n = 1..10 */
    static const seq_t base[10] = {
        (seq_t)9,          (seq_t)81,          (seq_t)289,
        (seq_t)1369,       (seq_t)5929,        (seq_t)26569,
        (seq_t)117649,     (seq_t)522729,      (seq_t)2319529,
        (seq_t)10297681
    };

    if (n == 0) return (seq_t)0;   /* undefined offset, return 0 */

    /* initialise cache with the first 10 terms */
    if (cache_len == 0) {
        cache_len = 10;
        /* allocate indices 0..10 (index 0 unused) */
        cache = (seq_t*)calloc(cache_len + 1, sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = base[i - 1];
    }

    if (n <= cache_len)
        return cache[n];

    /* enlarge the cache to hold entries up to n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        /* positive contributions */
        val += (seq_t)4   * cache[i - 1];
        val +=               cache[i - 2];
        val += (seq_t)18  * cache[i - 4];
        val += (seq_t)4   * cache[i - 5];
        val +=               cache[i - 10];
        /* negative contributions */
        val -= (seq_t)2   * cache[i - 6];
        val -= (seq_t)8   * cache[i - 7];
        val -=               cache[i - 8];
        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
