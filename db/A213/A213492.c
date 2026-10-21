/*
 * A213492  Number of (w,x,y) with all terms in {0,...,n} and w != min(|w-x|,
 *          |x-y|, |y-w|).
 * https://oeis.org/A213492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - a(n-4) + 2*a(n-6) - a(n-7) (n>=7)
 *            with base values a(0)=0, a(1)=4, a(2)=18, a(3)=48, a(4)=98,
 *            a(5)=178, a(6)=290
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_size = 0;

static void ensure_capacity(idx_t needed)
{
    if (cache_capacity >= (size_t)needed)
        return;
    size_t new_cap = cache_capacity ? cache_capacity << 1 : 8;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    ensure_capacity(n + 1);

    static const seq_t base[7] = {0, 4, 18, 48, 98, 178, 290};

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 6)
            cache[i] = base[i];
        else
            cache[i] = (seq_t)2 * cache[i - 1]
                     - cache[i - 3]
                     - cache[i - 4]
                     + (seq_t)2 * cache[i - 6]
                     - cache[i - 7];
    }

    cache_size = n + 1;
    return cache[n];
}
