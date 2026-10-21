/*
 * A085015  Multiplicity of the root 1 in the characteristic polynomial mod 2
 *          of the n X n matrix with entries binomial(i+j,i), 0<=i,j<n.
 * https://oeis.org/A085015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0 a(2^l - k) = (2^l + 2*(-1)^l)/3 - k + 2*a(k) for 0 ≤ k
 *            ≤ 2^{l-1}, where l = ⌈log₂(n)⌉ and k = 2^l - n. The recursion is
 *            evaluated only for n > 0. It runs in O(log n) time and needs no
 *            global state. For n = 0 the value is 0
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   a_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for int64_t */
#include "../utils/a_recursive.h"

seq_t a(idx_t n)
{
    return a_recursive(n);
}
