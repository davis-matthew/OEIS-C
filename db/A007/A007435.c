/*
 * A007435  Inverse Moebius transform of Fibonacci numbers 1,1,2,3,5,8,...
 * https://oeis.org/A007435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} Fibonacci(d)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 870 (2026-11-01)
 * Helpers:   fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fibonacci.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            result += fibonacci(d);
            if (d != n / d) {
                result += fibonacci(n / d);
            }
        }
    }
    
    return result;
}
