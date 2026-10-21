/*
 * A089049  Number of ways of writing n as a sum of exactly 4 powers of 2.
 * https://oeis.org/A089049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of exactly 4 powers of
 *            2 (powers may repeat, order is ignored)
 * Range:     u8 n <= 9719, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper: floor(log2(n)) for unsigned 64‑bit integers */
static unsigned _floor_log2_uint64(idx_t n)
{
    unsigned r = 0;
    while (n >>= 1U)
        ++r;
    return r;
}

/* Cache of computed values.  cache[i] == a(i). */
static seq_t *cache = NULL;     /* pointer to array of seq_t */
static idx_t cache_size = 0;    /* number of entries that are valid (== cache_size-1 is max computed) */

/* Extend the cache to cover all n up to 'max_n'.  The old contents are
   discarded; the values are recomputed from scratch. */
static void _extend_cache(idx_t max_n)
{
    if (max_n < cache_size)
        return;                     /* already covered */

    /* Allocate new array, zero‑initialized */
    seq_t *new_cache = (seq_t*)calloc((size_t)(max_n + 1), sizeof(seq_t));
    if (!new_cache)
        return;                     /* allocation failed – leave cache unchanged */

    /* Determine the largest exponent needed: 2^max_exp <= max_n < 2^(max_exp+1) */
    unsigned max_exp = 0;
    if (max_n > 0) {
        max_exp = _floor_log2_uint64(max_n);
    }

    /* Pre‑compute powers of two: pow2[e] = 2^e */
    idx_t *pow2 = (idx_t*)malloc((size_t)(max_exp + 1) * sizeof(idx_t));
    if (!pow2) {                      /* allocation failed – clean up */
        free(new_cache);
        return;
    }
    pow2[0] = (idx_t)1;
    for (unsigned e = 1; e <= max_exp; ++e)
        pow2[e] = pow2[e - 1] << 1U;

    /* Enumerate all unordered quadruples of exponents a<=b<=c<=d */
    for (unsigned a = 0; a <= max_exp; ++a) {
        idx_t pa = pow2[a];
        for (unsigned b = a; b <= max_exp; ++b) {
            idx_t sb = pa + pow2[b];
            if (sb > max_n)
                break;
            for (unsigned c = b; c <= max_exp; ++c) {
                idx_t sc = sb + pow2[c];
                if (sc > max_n)
                    break;
                for (unsigned d = c; d <= max_exp; ++d) {
                    idx_t sum = sc + pow2[d];
                    if (sum > max_n)
                        break;
                    ++new_cache[(size_t)sum];
                }
            }
        }
    }

    /* Replace old cache */
    free(pow2);
    free(cache);
    cache = new_cache;
    cache_size = max_n + 1;
}

seq_t a(idx_t n)
{
    /* Ensure the cache contains all values up to n */
    _extend_cache(n);
    return cache[(size_t)n];
}
