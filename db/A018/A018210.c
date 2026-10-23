/*
 * A018210  Alkane (or paraffin) numbers l(9,n).
 * https://oeis.org/A018210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (n+1)(n+2)(n+3)^2(4n^2+6n+5)/90 a(2n-1) =
 *            n(n+1)(n+2)(n+3)(4n^2+6n+5)/90
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   compute_even, compute_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_even.h"
#include "../utils/compute_odd.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        idx_t m = n >> 1;
        return compute_even(m);
    } else {
        idx_t m = (n >> 1) + 1;
        return compute_odd(m);
    }
}
