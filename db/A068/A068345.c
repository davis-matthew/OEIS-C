/*
 * A068345  Powers of 2, the fourth-from-last digit of which equals 7.
 * https://oeis.org/A068345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth power of 2 whose fourth-from-last decimal digit
 *            equals 7
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t count = 0;              /* how many qualifying powers found */
    idx_t k = 1;                  /* current exponent */
    uint32_t mod = 2 % 10000;     /* 2^k mod 10000 */

    while (1) {
        /* thousands digit (fourth from the end) is (mod / 1000) % 10 */
        if ((mod / 1000U) % 10U == 7U) {
            ++count;
            if (count == n)
                return pow2(k);
        }

        ++k;
        mod = (uint32_t)((mod * 2U) % 10000U);
    }
}
