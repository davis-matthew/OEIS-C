/*
 * A212899  Number of (w,x,y,z) with all terms in {0,...,n} and (least
 *          gapsize)>2.
 * https://oeis.org/A212899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
 *            (n>=10) a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=16, a(5)=62,
 *            a(6)=186, a(7)=456, a(8)=962, a(9)=1818. This recurrence is
 *            equivalent to the generating function denominator
 *            1‑5x+10x²‑10x³+5x⁴‑x⁵
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 258, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* cache for already computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed = 0;   /* largest index that has been computed */
static bool initialized = false;

/* ensure that the cache can accommodate at least `need` elements */
static void ensure_capacity(idx_t need)
{
    if (cache_len >= need) return;
    idx_t new_cap = cache_len ? cache_len * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    void *p = realloc(cache, new_cap * (sizeof(seq_t)));
    cache = (seq_t *)p;
    cache_len = new_cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[10] = {0, 0, 0, 2, 16, 62, 186, 456, 962, 1818};

    if (n < 10)
        return init[(idx_t)n];

    ensure_capacity(n + 1);

    if (!initialized) {
        for (idx_t i = 0; i < 10; ++i) cache[i] = init[i];
        computed = 9;
        initialized = true;
    }

    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t val = (seq_t)5 * cache[i - 1]
                     - (seq_t)10 * cache[i - 2]
                     + (seq_t)10 * cache[i - 3]
                     - (seq_t)5  * cache[i - 4]
                     + cache[i - 5];
            cache[i] = val;
        }
        computed = n;
    }

    return cache[n];
}
