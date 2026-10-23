/*
 * A126478  Number of base 10 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-1) - 4*a(n-2) - 10*a(n-3) + 6*a(n-4) Initial
 *            values: a(0)=1, a(1)=10, a(2)=58, a(3)=350, a(4)=2130
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Handle the small indices directly */
    if (n == 0) return 1;
    if (n == 1) return 10;
    if (n == 2) return 58;
    if (n == 3) return 350;
    if (n == 4) return 2130;

    /* Ensure cache is allocated and contains the base case values */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */
        cache[0] = 1;
        cache[1] = 10;
        cache[2] = 58;
        cache[3] = 350;
        cache[4] = 2130;
    }

    /* Grow cache if necessary and compute values up to n */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        cache = tmp;

        /* Compute residues from the current size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 7 * cache[i - 1];
            val -= 4 * cache[i - 2];
            val -= 10 * cache[i - 3];
            val += 6 * cache[i - 4];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
