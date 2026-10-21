/*
 * A074854  a(n) = Sum_{d|n} (2^(n-d)).
 * https://oeis.org/A074854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} (2^(n-d))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

/* Compute a(n) for n >= 1.  Complexity O(sqrt(n)). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += pow2(n - i);
            idx_t j = n / i;
            if (j != i) {
                sum += pow2(n - j);
            }
        }
    }
    return sum;
}
