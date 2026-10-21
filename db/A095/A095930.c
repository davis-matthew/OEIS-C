/*
 * A095930  Number of walks of length 2n between two nodes at distance 2 in
 *          the cycle graph C_10.
 * https://oeis.org/A095930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 13*a(n-2) + 4*a(n-3) for n >= 4 with a(1) = 1,
 *            a(2) = 4, a(3) = 15
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cap(idx_t need)
{
    if (need + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* initialize new elements to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n < 1, but not used */
    ensure_cap(n);

    /* compute base cases */
    if (n == 1) { cache[1] = 1; return 1; }
    if (n == 2) { cache[2] = 4; return 4; }
    if (n == 3) { cache[3] = 15; return 15; }

    /* fill missing lower terms if any */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != 0) continue;          /* already computed */
        cache[i] = (7 * cache[i - 1]) - 
                   (13 * cache[i - 2]) + 
                   (4 * cache[i - 3]);
    }

    return cache[n];
}
