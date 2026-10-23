/*
 * A023967  First digit after decimal point of 8th root of n.
 * https://oeis.org/A023967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10 * (n^(1/8) - floor(n^(1/8)))) for n ≥ 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_root8_idx, pow8_ui128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_root8_idx.h"
#include "../utils/pow8_ui128.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 by the OEIS offset; nevertheless
       handle 0 gracefully for safety. */
    if (n == 0) return 0;

    /* Step 1: integer part of nth root */
    idx_t r = floor_root8_idx(n);          /* floor(n^(1/8)) */

    /* Step 2: compute product 10^8 * n in 128 bits to avoid overflow */
    const unsigned __int128 prod = (unsigned __int128)n * 100000000ULL; /* 10^8 * n */

    /* Step 3: a = 10 * r */
    idx_t a10 = r * 10;

    /* Step 4: find the largest d in 0..9 such that (a10 + d)^8 ≤ prod */
    idx_t dmax = 0;
    for (idx_t d = 0; d <= 9; ++d)
    {
        unsigned int val = (unsigned int)(a10 + d);
        unsigned __int128 pow_val = pow8_ui128(val);
        if (pow_val > prod)
            break;
        dmax = d;
    }

    return (seq_t)dmax;   /* digit is in 0..9, safe for all seq_t sizes */
}
