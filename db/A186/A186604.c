/*
 * A186604  Number of (n+4)X8 binary arrays with every 5X5 subblock commuting
 *          with each horizontal and vertical neighbor 5X5 subblock.
 * https://oeis.org/A186604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) - a(n-6) - a(n-8) +
 *            a(n-9) + a(n-10) - a(n-13) (for n > 18), with the first 18 terms
 *            a(1)=42120, a(2)=3455, a(3)=5813, a(4)=19248, a(5)=54040,
 *            a(6)=90664, a(7)=98376, a(8)=99820, a(9)=101072, a(10)=102332,
 *            a(11)=103620, a(12)=105172, a(13)=108332, a(14)=11...
 * Range:     u8 none, u16 n <= 5, u32 n <= 61, u64 n <= 117, u128 n <= 117,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/ensure_cache.h"  /* for ensure_cache() */

static const seq_t init18[18] = {
    (seq_t)42120, (seq_t)3455, (seq_t)5813, (seq_t)19248,
    (seq_t)54040, (seq_t)90664, (seq_t)98376, (seq_t)99820,
    (seq_t)101072, (seq_t)102332, (seq_t)103620, (seq_t)105172,
    (seq_t)108332, (seq_t)117492, (seq_t)152956, (seq_t)244561,
    (seq_t)344142, (seq_t)446775
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined, guard against misuse */

    static seq_t *cache = NULL;          /* cache[i] holds a(i+1) */
    static size_t cache_cap = 0;
    static size_t done = 0;              /* number of entries already computed */

    /* Ensure capacity for the requested index (1‑based). */
    ensure_cache(n, &cache, &cache_cap);

    /* Fill in the initial 18 values if not already done. */
    if (done < 18) {
        size_t need = (n < 18 ? n : 18);
        for (size_t i = done; i < need; ++i) {
            cache[i] = init18[i];
        }
        done = need;
    }

    /* If the needed value is already cached, return it. */
    if (n <= done) {
        return cache[n - 1];
    }

    /* Compute the remaining values up to n using the recurrence. */
    for (idx_t i = done + 1; i <= n; ++i) {   /* i is 1‑based index */
        /* Indices in cache are 0‑based: a(k) == cache[k-1] */
        seq_t val = cache[i - 2];              /* a(i-1) */
        val += cache[i - 4];                   /* a(i-3) */
        val -= cache[i - 5];                   /* a(i-4) */
        val += cache[i - 6];                   /* a(i-5) */
        val -= cache[i - 7];                   /* a(i-6) */
        val -= cache[i - 9];                   /* a(i-8) */
        val += cache[i -10];                   /* a(i-9) */
        val += cache[i -11];                   /* a(i-10) */
        val -= cache[i -14];                   /* a(i-13) */
        cache[i - 1] = val;
    }
    done = n;
    return cache[n - 1];
}
