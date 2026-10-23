/*
 * A108791  a(2n) = -5*(fibonacci(6n+2))^2, a(2n+1) = (lucas(6n+5))^2.
 * https://oeis.org/A108791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(2*n) = -5*(fibonacci(6*n+2))^2, a(2*n+1) = (lucas(6*n+5))^2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 14, i128 n <= 29,
 *            big any n
 * Helpers:   fibonacci, lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fibonacci.h"
#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    seq_t res;
    if ((n & 1) == 0)          /* even n: use Fibonacci */
    {
        idx_t m = n >> 1;
        seq_t f = fibonacci(6 * m + 2);
        res = -5 * f * f;
    }
    else                       /* odd n: use Lucas */
    {
        idx_t m = (n - 1) >> 1;
        seq_t l = lucas(6 * m + 5);
        res = l * l;
    }
    return res;
}
