/*
 * A188104  Number of (n+3)X11 binary arrays with every 4X4 subblock commuting
 *          with each horizontal and vertical neighbor 4X4 subblock.
 * https://oeis.org/A188104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) + a(n-8) -
 *            a(n-9) for n > 18 Initial terms (basis of the recurrence):
 *            a(1)=20888, a(2)=3110, a(3)=4285, a(4)=6253, a(5)=9265,
 *            a(6)=11245, a(7)=12477, a(8)=14853, a(9)=17589, a(10)=20717,
 *            a(11)=24669, a(12)=30593, a(13)=40709, a(14)=52461, a(15...
 * Range:     u8 none, u16 n <= 14, u32 n <= 59, u64 n <= 149, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* zero‑based cache storing a(1)..a(n) */
static idx_t cache_len = 0;      /* number of cached elements */

/* Basis values for n = 1 .. 18  (zero‑based indices 0 .. 17) */
static const seq_t base_vals[18] = {
    (seq_t)20888, (seq_t)3110, (seq_t)4285, (seq_t)6253,  (seq_t)9265,
    (seq_t)11245, (seq_t)12477, (seq_t)14853, (seq_t)17589, (seq_t)20717,
    (seq_t)24669, (seq_t)30593, (seq_t)40709, (seq_t)52461, (seq_t)66065,
    (seq_t)83805, (seq_t)108473, (seq_t)140714
};

/* Ensure cache contains values up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* Allocate or enlarge the cache. */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!cache) abort();
    } else if ((size_t)n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Fill missing terms. */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i < 18) {                    /* basis values */
            cache[i] = base_vals[i];
        } else {                         /* recurrence for n > 18 */
            seq_t val =  2 * cache[i-1];
            val -= cache[i-2];
            val += cache[i-4];
            val -= 2 * cache[i-5];
            val += cache[i-6];
            val += cache[i-8];
            val -= cache[i-9];
            cache[i] = val;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 – return 0 */
    ensure_cache(n);
    return cache[n-1];
}
