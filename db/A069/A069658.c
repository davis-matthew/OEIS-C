/*
 * A069658  a(1) = 1; a(n) = smallest nontrivial n-digit perfect power.
 * https://oeis.org/A069658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (floor(sqrt(10^(n-1))) + 1)^2 for n ≥ 2, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   floor_sqrt, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n == 1)
        return 1;

    /* 10^(n-1) */
    seq_t p = pow10(n - 1);

    /* floor(sqrt(10^(n-1))) */
    seq_t s = floor_sqrt(p);

    /* (s + 1)^2 */
    seq_t inc = s + 1;
    seq_t res = seq_mul(inc, inc);

    return res;
}
