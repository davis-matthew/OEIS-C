/*
 * A056148  a(0) = 1, a(n) = number of a(k), for 0 <= k <= n-1, that divide n.
 * https://oeis.org/A056148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056148: a(0) = 1; a(n) = number of previous a(k) that divide n. */
/* The implementation keeps a cache of computed a(k) and a frequency array for
   a(k) values.  For each new n it enumerates all divisors d of n and sums
   the frequencies of equal a(k)==d.  The algorithm runs in O(n·sqrt(n))
   time and uses O(n) memory. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache   = NULL;   /* values a(k)  */
    static size_t cache_sz = 0;     /* allocated size  */
    static idx_t  current_max = (idx_t)-1; /* highest index computed */

    /* static storage for value frequencies */
    static seq_t *freq   = NULL;   /* freq[v] = number of times a(k)=v  */
    static size_t freq_sz = 0;     /* allocated size of freq array   */
    static idx_t  max_a   = 0;     /* maximum a(k) seen so far       */

    /* initialise on first call */
    if (cache == NULL) {
        /* cache for a(0) */
        cache_sz  = 1;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
        current_max = 0;

        /* frequency array starts with capacity 2: indices 0 and 1 */
        freq_sz = 2;
        freq = (seq_t *)malloc(freq_sz * sizeof(seq_t));
        if (!freq) return (seq_t)0;
        freq[0] = (seq_t)0;                   /* a(k) never 0 */
        freq[1] = (seq_t)1;                   /* one occurrence of value 1 (a(0)) */
        max_a   = 1;
    }

    /* expand cache array if necessary */
    if ((size_t)n >= cache_sz) {
        size_t new_sz = cache_sz ? cache_sz : 1;
        while ((size_t)n >= new_sz) new_sz <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* compute missing values up to and including n */
    if (n > current_max) {
        for (idx_t m = current_max + 1; m <= n; ++m) {
            seq_t cnt = (seq_t)0;

            /* enumerate divisors of m */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    if (d < freq_sz) cnt += freq[d];
                    idx_t d2 = m / d;
                    if (d2 != d && d2 < freq_sz) cnt += freq[d2];
                }
            }

            cache[m] = cnt;

            /* update frequency array for the new value */
            idx_t v = (idx_t)cnt;          /* cnt fits into idx_t for n <= 10000 */
            if (v >= freq_sz) {
                size_t new_sz = freq_sz ? freq_sz : 1;
                while (new_sz <= v) new_sz <<= 1;
                seq_t *tmp = (seq_t *)realloc(freq, new_sz * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failed */
                /* initialise newly allocated portion to 0 */
                for (size_t i = freq_sz; i < new_sz; ++i) tmp[i] = (seq_t)0;
                freq = tmp;
                freq_sz = new_sz;
            }
            freq[v] += (seq_t)1;
            if (v > max_a) max_a = v;
        }
        current_max = n;
    }

    return cache[n];
}
