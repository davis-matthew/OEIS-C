/*
 * A218148  a(n) = 2^((6+5*n+n^3)/6).
 * https://oeis.org/A218148
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((6+5*n+n^3)/6)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided pow2 utility to compute 2^exp. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Undefined behaviour below the offset; 0 is returned for safety. */
    if (n < -1) {
        return 0;
    }

    /* Handle the offset value explicitly. */
    if (n == -1) {
        return pow2(0); /* a(-1) = 1 */
    }

    /* Compute the exponent using 64‑bit arithmetic to avoid overflow
       for the values tested (n up to about 10^4). */
    idx_t nn = n;                /* cast to idx_t for clarity */
    idx_t exp = (6 + 5 * nn + nn * nn * nn) / 6;

    return pow2(exp);
}
