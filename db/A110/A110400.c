/*
 * A110400  a(1) = 1, a(2) = 21; for n>2, a(n) is the least number whose
 *          neighboring digit difference forms a(n-1) such that a(n+1) also
 *          exists.
 * https://oeis.org/A110400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1 a(2) = 21 for n ≥ 3: if n is odd : a(n) = 10^(n-1) + 32
 *            if n is even : a(n) = 10^(n-1) + 31 (where 10^(k) is computed
 *            with utils/pow10.h)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)21;

    seq_t val = pow10(n - 1);
    if (n & 1U)          /* odd */
        return val + (seq_t)32;
    else                 /* even */
        return val + (seq_t)31;
}
