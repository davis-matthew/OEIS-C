/*
 * A080918  Number of integer solutions to the equation 2x^2+y^2+32z^2=n.
 * https://oeis.org/A080918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 683, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of integer solutions to 2x^2 + y^2 + 32z^2 = n,
   counting all sign choices.  The routine enumerates the
   possible values of z and x, checks whether the remaining
   value is a perfect square in y, and accumulates the
   multiplicities arising from the signs.  Overflow is avoided
   by using 128‑bit arithmetic for the intermediate square
   computations, which is safe for all 64‑bit values of n. */

#include "../utils/_isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = n;               /* n as seq_t for subtraction */

    for (idx_t z = 0; ; ++z) {
        /* 32*z^2, computed in 128‑bit to avoid overflow */
        __uint128_t zsq128 = (__uint128_t)z * z * 32;
        if (zsq128 > (uint64_t)n) break;
        seq_t zsq = (seq_t)zsq128;
        seq_t nz  = n_seq - zsq;   /* remaining after 32z^2   */
        uint64_t nz64 = (uint64_t)nz;   /* fits into 64 bits (n≤2^64‑1) */

        for (idx_t x = 0; ; ++x) {
            /* 2*x^2, computed in 128‑bit to avoid overflow */
            __uint128_t xsq128 = (__uint128_t)x * x * 2;
            if (xsq128 > nz64) break;
            seq_t xsq = (seq_t)xsq128;
            seq_t rem = nz - xsq;   /* remaining after 2x^2     */

            seq_t sq = _isqrt_seq(rem);
            if (sq * sq == rem) {
                seq_t z_mult = (z == 0 ? (seq_t)1 : (seq_t)2);
                seq_t x_mult = (x == 0 ? (seq_t)1 : (seq_t)2);
                seq_t y_mult = (rem == 0 ? (seq_t)1 : (seq_t)2);
                count += z_mult * x_mult * y_mult;
            }
        }
    }
    return count;
}
