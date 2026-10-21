/*
 * A074188  Smallest power (>=2) >= n!.
 * https://oeis.org/A074188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest perfect power >= n! (exponents ≥2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt, seq_alloc_new_array, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_pow.h"

/* Compute the sequence A074188 */
seq_t a(idx_t n)
{
    /* Cache of factorials up to the largest n requested (max 10000) */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cap = 0;          /* largest index computed so far */

    /* Allocate cache once (size 10001 to hold 0…10000) */
    if (!fact_cache) {
        size_t max_n = 10000;
        fact_cache = seq_alloc_new_array(max_n + 1);
        fact_cache[0] = 1;           /* 0! = 1 */
        fact_cap = 0;
    }

    /* Extend factorial cache up to n if necessary */
    if (n > fact_cap) {
        for (idx_t i = fact_cap + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;  /* factorial[i] = factorial[i-1] * i */
        }
        fact_cap = n;
    }

    seq_t F = fact_cache[n];          /* F = n! */

    /* Smallest square >= F */
    seq_t t = floor_sqrt(F);
    seq_t sq = t * t;
    if (sq < F) {
        t = t + (seq_t)1;
        sq = t * t;
    }

    /* Smallest cube >= F */
    seq_t c = floor_cube_root(F);
    seq_t cb = seq_pow(c, 3);
    if (cb < F) {
        c = c + (seq_t)1;
        cb = seq_pow(c, 3);
    }

    /* Return the smaller of the two candidates */
    seq_t ans = sq;
    if (cb < ans) ans = cb;
    return ans;
}
