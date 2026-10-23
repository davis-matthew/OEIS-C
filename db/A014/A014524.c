/*
 * A014524  Number of Hamiltonian paths from NW to SW corners in a grid with
 *          2n rows and 4 columns.
 * https://oeis.org/A014524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)-9*a(n-2)+7*a(n-3)-a(n-4) with initial values
 *            a(0)=0, a(1)=1, a(2)=8, a(3)=47
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed = 0;   /* number of indices computed (0..computed-1) */

    size_t nn = (size_t)n;

    /* Ensure allocated capacity */
    if (nn + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < nn + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base cases once */
    if (!computed) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 8;
        cache[3] = 47;
        computed = 4;
    }

    /* Compute missing values up to nn */
    if (computed <= nn) {
        for (size_t k = computed; k <= nn; ++k) {
            cache[k] = 7 * cache[k-1] - 9 * cache[k-2] + 7 * cache[k-3] - cache[k-4];
        }
        computed = nn + 1;
    }

    return cache[nn];
}
