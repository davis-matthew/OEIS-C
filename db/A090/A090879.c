/*
 * A090879  a(n) = Sum_{d|n} d*2^(n-d).
 * https://oeis.org/A090879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} d * 2^(n-d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            sum += (seq_t)d1 * pow2(n - d1);
            idx_t d2 = n / i;
            if (d2 != d1) {
                sum += (seq_t)d2 * pow2(n - d2);
            }
        }
    }
    return sum;
}
