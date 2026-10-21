/*
 * A089274  Fifth column of the Legendre-Stirling triangle A071951.
 * https://oeis.org/A089274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70*a(n-1) - 1708*a(n-2) + 17544*a(n-3) - 72000*a(n-4) +
 *            86400*a(n-5) (for n ≥ 5). Initial values: a(0)=1, a(1)=70,
 *            a(2)=3192, a(3)=121424, a(4)=4203824
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* storage for computed values   */
static size_t cache_len = 0;     /* number of elements already stored */

/* Ensure that cache holds at least first 5 terms. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)70;
        cache[2] = (seq_t)3192;
        cache[3] = (seq_t)121424;
        cache[4] = (seq_t)4203824;
    }
}

/* Grow cache to contain indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_len) return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (tmp == NULL) abort();
    cache = tmp;
    /* compute new terms */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];

        seq_t val = (seq_t)70    * t1;
        val -= (seq_t)1708   * t2;
        val += (seq_t)17544  * t3;
        val -= (seq_t)72000  * t4;
        val += (seq_t)86400  * t5;

        cache[i] = val;
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n < 5) return cache[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
