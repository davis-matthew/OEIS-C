/*
 * A133018  Partition number of n, raised to power n.
 * https://oeis.org/A133018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partition(n)^n Partition numbers are computed via Euler’s
 *            generalized pentagonal recurrence. The cache stores all
 *            partition values computed so far so subsequent calls are O(1)
 *            for already-computed indices
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache for partition numbers */
    static seq_t *part_cache = NULL;   /* p[0] .. p[max_n] */
    static size_t cache_cap  = 0;      /* allocated size of part_cache   */
    static idx_t  cache_max  = 0;      /* largest n for which p[n] computed */

    /* Resize cache if necessary */
    if (!part_cache || (size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(part_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        /* initialise new elements to 0 (safe for unsigned and big ints) */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        part_cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the base case if not yet done */
    if (cache_max == 0) {
        if (part_cache[0] == 0) part_cache[0] = 1;
        cache_max = 0;
    }

    /* compute partition numbers up to n using Euler's recurrence */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = (idx_t)k * (3 * k - 1) / 2;   /* generalized pentagonal numbers */
            if (g1 > m) break;

            seq_t t = part_cache[m - g1];

            if ((k & 1) == 0) sum -= t;  /* even k : negative sign */
            else             sum += t;  /* odd  k : positive sign */

            idx_t g2 = (idx_t)k * (3 * k + 1) / 2;
            if (g2 > m) continue;

            t = part_cache[m - g2];

            if ((k & 1) == 0) sum -= t;
            else             sum += t;
        }
        part_cache[m] = sum;
    }
    cache_max = n;

    /* compute partition(n)^n */
    return big_pow(part_cache[n], (seq_t)n);
}
