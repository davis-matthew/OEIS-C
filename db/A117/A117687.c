/*
 * A117687  Squares for which both the sum of the digits and the product of
 *          the digits are cubes.
 * https://oeis.org/A117687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 3445, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_product, digit_sum, floor_cube_root,
 *            seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117687: squares whose digit sum and digit product are cubes (brute‑force). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_mul.h"
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/seq_pow.h"

/* Return true iff x is a perfect cube. */
static inline bool is_cube(seq_t x)
{
    if (x == 0) return true;
    seq_t r = floor_cube_root(x);
    return seq_pow(r, (idx_t)3) == x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety guard for out‑of‑range */

    /* Static storage for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;       /* number of terms stored */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t next_m = 0;           /* next integer to be squared */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        seq_t m = (seq_t)next_m;
        seq_t square = seq_mul(m, m);

        seq_t sum = digit_sum(square);
        seq_t prod = digit_product(square);

        if (is_cube(sum) && is_cube(prod))
            append_candidate(&cache, &cache_len, &cache_cap, square);

        ++next_m;
    }

    return cache[n - 1];
}
