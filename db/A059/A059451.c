/*
 * A059451  Number of ways n can be written as the sum of two numbers whose
 *          binary expansions have even numbers of zeros; also number of ways
 *          n can be written as the sum of two numbers whose binary expansions
 *          have odd numbers of zeros.
 * https://oeis.org/A059451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y): 0 ≤ x ≤ y, x + y = n, #0bits(x) mod 2 = 1,
 *            #0bits(y) mod 2 = 1}|. The binary representation of 0 is taken
 *            to have one zero bit
 * Range:     u8 n <= 1278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_zero_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/odd_zero_parity.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        if (odd_zero_parity(k) && odd_zero_parity(n - k))
            ++cnt;
    }
    return cnt;
}
