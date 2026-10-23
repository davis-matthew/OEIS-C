/*
 * A116476  Numbers k such that T(k) + T(k+1) + ... + T(k+10) is a square,
 *          where T(m) = A000217(m) is the m-th triangular number.
 * https://oeis.org/A116476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 394*a(n-4) - a(n-8) + 2156 for n > 8, with initial values
 *            a(1) = 13, a(2) = 46, a(3) = 229, a(4) = 1608, a(5) = 7335, a(6)
 *            = 20304, a(7) = 92391, a(8) = 635710
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t const initial[8] = {
    13,   46,   229,  1608,  7335,  20304,  92391,  635710
};

static seq_t *cache = NULL;      /* array of computed terms, 1‑based indexing */
/* cache_len: number of terms currently stored (cache[0] = a(1)) */
static size_t cache_len = 0;
/* cache_cap: allocated capacity of cache */
static size_t cache_cap = 0;

static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_size = cache_cap ? cache_cap*2 : 16;
    if (new_size < new_cap)
        new_size = new_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1, but return 0 */

    if (cache_len < n) {
        ensure_capacity(n);
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            if (i < 8) {
                cache[i] = initial[i];
            } else {
                /* Recurrence: a(n) = 394*a(n-4) - a(n-8) + 2156
                   zero‑based indices: cache[i] = a(i+1),
                   cache[i-4] = a((i+1)-4), cache[i-8] = a((i+1)-8) */
                cache[i] = (seq_t)394 * cache[i-4]
                           - cache[i-8]
                           + (seq_t)2156;
            }
        }
        cache_len = n;
    }
    return cache[n-1];
}
