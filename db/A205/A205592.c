/*
 * A205592  a(2) = 1, a(3k) = a(3k+1) = a(2k), a(3k+2) = 2a(2k+1) for k >= 1.
 * https://oeis.org/A205592
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2) = 1 a(3k) = a(3k+1) = a(2k) a(3k+2) = 2 * a(2k+1) (k ≥ 1)
 *            (recurrence definition of OEIS A205592)
 * Range:     u8 n <= 241, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array indexed by n, 0 .. max_n */
static size_t cache_len = 0;     /* current allocated size (max_n+1) */

static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp && new_len > 0) {          /* allocation failed – abort */
        exit(1);
    }
    cache = tmp;
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 2) {
            cache[i] = (seq_t)1;
        } else {
            idx_t mod = i % 3;
            if (mod == 0) {            /* a(3k)   = a(2k)           */
                idx_t k = i / 3;
                cache[i] = cache[2 * k];
            } else if (mod == 1) {     /* a(3k+1) = a(2k)           */
                idx_t k = (i - 1) / 3;
                cache[i] = cache[2 * k];
            } else {                   /* mod == 2 : a(3k+2) = 2*a(2k+1) */
                idx_t k = (i - 2) / 3;
                cache[i] = (seq_t)2 * cache[2 * k + 1];
            }
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2 – callers use n ≥ 2 */
    ensure_cache(n);
    return cache[n];
}
