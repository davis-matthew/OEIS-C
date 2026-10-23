/*
 * A046995  Number of Greek-key tours on a 4 X n board; i.e., self-avoiding
 *          walks on 4 X n grid starting in top left corner.
 * https://oeis.org/A046995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) =
 *            3*a(n-1)+3*a(n-2)-9*a(n-3)-6*a(n-4)+5*a(n-5)+a(n-6)-3*a(n-7)+a(n-8)
 *            for n≥10 Initial values: a(1)=1, a(2)=4, a(3)=17, a(4)=52,
 *            a(5)=160, a(6)=469, a(7)=1337, a(8)=3750, a(9)=10347
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static on‑demand cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Return 0 for indices below the offset */
    if (n < 1) return 0;

    /* Ensure cache is at least large enough for the first 9 values */
    if (cache_size < 9)
    {
        size_t needed = 10;                     /* indices 0..9 (0 unused) */
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache[0] = 0;
        /* Initial terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)52;
        cache[5] = (seq_t)160;
        cache[6] = (seq_t)469;
        cache[7] = (seq_t)1337;
        cache[8] = (seq_t)3750;
        cache[9] = (seq_t)10347;
        cache_size = 9;
    }

    /* If requested term is already cached, return it */
    if (n <= cache_size) return cache[n];

    /* Grow cache to accommodate n */
    size_t new_cap = (size_t)(n + 1);           /* indices 0..n */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    /* Compute missing values using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t r = 0;
        r += 3 * cache[i - 1];
        r += 3 * cache[i - 2];
        r -= 9 * cache[i - 3];
        r -= 6 * cache[i - 4];
        r += 5 * cache[i - 5];
        r +=     cache[i - 6];
        r -= 3 * cache[i - 7];
        r +=     cache[i - 8];
        cache[i] = r;
    }
    cache_size = n;
    return cache[n];
}
