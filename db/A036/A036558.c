/*
 * A036558  Squared distances that can arise in any Barlow packing of spheres
 *          of diameter sqrt(3) (the scale being the simplest one that makes
 *          this an integer sequence).
 * https://oeis.org/A036558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth non‑negative integer N such that N = x*x + x*y +
 *            y*y + 2*z*z for some integers x,y,z, with the proviso that if
 *            |z| < 2 then N must be a multiple of 3
 * Range:     u8 n <= 46, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   representable_norm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/representable_norm.h"

/* Integer square root of a value stored in seq_t.
   Returns ⌊sqrt(x)⌋. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi) {
        seq_t mid = (lo + hi) >> 1;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Test whether N can be expressed in the required form. */
static bool representable(seq_t N)
{
    seq_t halfN = N >> 1;             /* floor(N/2) */
    idx_t max_z = (idx_t)isqrt_seq(halfN);
    for (idx_t z = 0; z <= max_z; ++z) {
        seq_t zsq = (seq_t)z * (seq_t)z;
        seq_t rest = N - ((seq_t)2 * zsq);
        if (z < 2 && (N % 3)) continue;     /* N must be divisible by 3 */
        if (representable_norm(rest)) return true;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t N = 0;
    while (true) {
        if (representable(N)) {
            if (count == n) return N;
            ++count;
        }
        ++N;
    }
}
