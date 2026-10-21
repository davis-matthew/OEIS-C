/*
 * A067439  a(n) = sum of all the remainders when n is divided by positive
 *          integers less than and coprime to n.
 * https://oeis.org/A067439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 <= d < n, gcd(d,n)=1} (n mod d)
 * Range:     u8 n <= 40, u16 n <= 612, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_gcd_idx.h"
/* Euclidean algorithm for idx_t (uint64_t) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t d = 1; d < n; ++d) {
        if (_gcd_idx(n, d) == 1)
            sum += (seq_t)(n % d);
    }
    return sum;
}
