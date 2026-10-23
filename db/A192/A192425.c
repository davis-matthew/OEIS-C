/*
 * A192425  Coefficient of x in the reduction by x^2 -> x+2 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(3)=6; for n≥4
 *            a(n)=a(n‑1)+4*a(n‑2)-a(n‑3)-a(n‑4)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void extend_to(size_t n)
{
    if (n < cache_len)
        return;
    /* Grow capacity to at least n+1 */
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);            /* allocation failure */
    cache = tmp;
    /* Initialize new entries */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 1;
        else if (i == 2)
            cache[i] = 1;
        else if (i == 3)
            cache[i] = 6;
        else {
            seq_t c4 = 4;
            seq_t val = cache[i-1] + cache[i-2] * c4 - cache[i-3] - cache[i-4];
            cache[i] = val;
        }
    }
    cache_len = n + 1;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    extend_to(idx);
    return cache[idx];
}
