/*
 * A158520  Numbers n such that 2^n divided by the number of digits of 2^n is
 *          an integer.
 * https://oeis.org/A158520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer k such that 2^k is divisible by the number of
 *            decimal digits of 2^k. The number of digits of 2^k is floor(k *
 *            log10(2)) + 1. This implementation computes the digit count
 *            using a high‑precision integer approximation of log10(2) and
 *            tests 2^k modulo that digit count
 * Range:     u8 n <= 25, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   pow_mod2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_mod2_uint64.h"

/*  log10(2) ≈ 0.3010299956639811952137389  (25 decimal places)
    represented as NUM/10^25 where NUM fits in unsigned __int128.  */
static const unsigned __int128 LOG2_NUM = ((unsigned __int128)163188ULL << 64)
                                         | (unsigned __int128)12684739297802025581ULL;
static const unsigned __int128 LOG2_DEN = 
    ((unsigned __int128)10000000000ULL) * 1000000000000000ULL;   /* 10^10 * 10^15 = 10^25 */

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (uint64_t k = 0; ; ++k)
    {
        /* compute number of decimal digits of 2^k */
        uint64_t d = (unsigned __int128)k * LOG2_NUM / LOG2_DEN + 1;

        /* check divisibility */
        if (pow_mod2_uint64(k, d) == 0)
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
