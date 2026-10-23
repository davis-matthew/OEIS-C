/*
 * A027946  a(n) is the sum of the non-Fibonacci numbers in row n of array T
 *          given by A027935, computed as T(n,m) + T(n,m+1) + ... + T(n,n-1),
 *          where m = floor((n+2)/2).
 * https://oeis.org/A027946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(n) = 2^n - 1 - Fibonacci(n+1) if n is odd a(n) = 2^n
 *            - 1 - Fibonacci(n+2) if n is even and n > 0 (Both formulas
 *            derived from the OEIS definition)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    seq_t val = pow2(n);         /* 2^n as seq_t */
    val -= (seq_t)1;             /* 2^n - 1 */

    if (n & 1) {                 /* odd n */
        val -= fib(n + 1);
    } else {                     /* even n (>0) */
        val -= fib(n + 2);
    }
    return val;
}
