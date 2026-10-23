/*
 * A128173  Numbers in ternary reflected Gray code order.
 * https://oeis.org/A128173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) enumerates numbers in ternary reflected Gray code order. *
 *            Recursively: let b be the largest power of 3 ≤ n. * Let k = n /
 *            b (0 ≤ k ≤ 2) and r = n % b. * Then * a(n) = k*b + a(r) if k is
 *            even * a(n) = k*b + a(b-1-r) if k is odd * Base case: a(0)=0,
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 242, u16 n <= 9925, u32 n <= 9925, u64 n <= 9925,
 *            u128 n <= 9925, big any n
 * Helpers:   _ternary_gray
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_ternary_gray.h"

seq_t a(idx_t n)
{
    return _ternary_gray(n);
}
