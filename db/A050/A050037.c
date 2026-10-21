/*
 * A050037  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A050037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(m) with m = 2^(p+1)+2 - n, where p is the
 *            unique integer such that 2^p < n-1 <= 2^(p+1); starting values:
 *            a(1)=a(2)=1, a(3)=4
 * Range:     u8 n <= 19, u16 n <= 72, u32 n <= 515, u64 n <= 4331,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache   = NULL;   /* indices start at 1 */
static idx_t  cache_cap = 0;    /* allocated size (index 0..cache_cap-1) */
static idx_t  computed = 0;     /* highest index computed so far */

/* ensure the cache can hold indices up to n */
static void
ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined, but guard against accidental use */

    if (n > computed) {
        /* compute missing values up to n in increasing order */
        ensure_cache(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1 || i == 2) {
                cache[i] = 1;
            } else if (i == 3) {
                cache[i] = 4;
            } else {
                /* find p such that 2^p < i-1 <= 2^(p+1) */
                idx_t power = 1ULL;   /* 2^0 */
                idx_t p = 0;
                while ((power << 1) < (i - 1)) {
                    power <<= 1;
                    ++p;
                }
                /* compute m = 2^(p+1) + 2 - i */
                idx_t m = (power << 1) + 2 - i;
                cache[i] = cache[i - 1] + cache[m];
            }
        }
        computed = n;
    }
    return cache[n];
}
