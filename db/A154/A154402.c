/*
 * A154402  Inverse Moebius transform of Fredholm-Rueppel sequence, cf.
 *          A036987.
 * https://oeis.org/A154402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of divisors d of n such that d + 1 is a power of
 *            two. * That is, a(n) = |{ d | d | n and (d + 1) & d == 0 }|
 * Range:     u8 n <= 7940, u16 n <= 7940, u32 n <= 7940, u64 n <= 7940,
 *            u128 n <= 7940, big any n
 * Helpers:   is_pow2_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_pow2_minus1.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard for safety */

    seq_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            if (is_pow2_minus1(i))
                ++count;
            idx_t j = n / i;
            if (j != i && is_pow2_minus1(j))
                ++count;
        }
    }
    return count;
}
