/*
 * A151281  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0) and consisting of n steps taken from {(-1, 0), (1, 0), (1,
 *          1)}.
 * https://oeis.org/A151281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(0)=1; a(2n+1)=3*a(2n)-2^n*C_n (C_n = n-th Catalan number)
 *            a(2n+2)=3*a(2n+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t target_half = n / 2;
    bool   is_odd = (bool)(n & 1U);

    seq_t a_even = 1;                /* Holds a(2i) in the loop */

    for (idx_t i = 0; ; ++i) {
        /* Even case: return a_even when we have reached the target index */
        if (!is_odd && i == target_half)
            return a_even;

        /* 2^i and C_i (Catalan) */
        seq_t pow2i    = pow2(i);
        seq_t cat_i    = catalan(i);
        seq_t pow2i_cat_i = pow2i * cat_i;

        /* a(2i+1) = 3*a_even - 2^i*C_i */
        seq_t a_odd = a_even * 3 - pow2i_cat_i;

        if (is_odd && i == target_half)
            return a_odd;

        /* Prepare a(2i+2) for next iteration */
        a_even = a_odd * 3;
    }
}
