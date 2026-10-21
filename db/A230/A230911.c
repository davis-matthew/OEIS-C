/*
 * A230911  Number of n X 4 0..7 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 8, and upper left element zero.
 * https://oeis.org/A230911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 91*a(n-2) + 469*a(n-3) - 919*a(n-4) +
 *            2152*a(n-5) - 93*a(n-6) + a(n-7), with a(1)=0, a(2)=0, a(3)=8,
 *            a(4)=118, a(5)=1506, a(6)=20028, a(7)=271450
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    if (cache == NULL) {
        /* Allocate space for indices 0..7 (0 unused) */
        cache = (seq_t *)malloc((7 + 1) * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0ull; /* a(1) */
        cache[2] = 0ull; /* a(2) */
        cache[3] = 8ull; /* a(3) */
        cache[4] = 118ull; /* a(4) */
        cache[5] = 1506ull; /* a(5) */
        cache[6] = 20028ull; /* a(6) */
        cache[7] = 271450ull; /* a(7) */
        max_computed = 7;
    }

    if (n <= max_computed) return cache[n];

    /* Ensure capacity up to n */
    size_t new_cap = (size_t)max_computed + 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    /* Compute terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t = (seq_t)18 * cache[i - 1];
        t += (seq_t)469 * cache[i - 3];
        t += (seq_t)2152 * cache[i - 5];
        t += cache[i - 7];

        seq_t s = (seq_t)91 * cache[i - 2];
        s += (seq_t)919 * cache[i - 4];
        s += (seq_t)93 * cache[i - 6];

        cache[i] = t - s; /* t >= s for all n >= 8 */
    }

    max_computed = n;
    return cache[n];
}
