/*
 * A078462  Partial sums of A035185.
 * https://oeis.org/A078462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A035185(k) where A035185(k) = Sum_{d|k}
 *            K(d,2), K(d,2) is the Kronecker symbol (d/2). Equivalently: a(n)
 *            = Sum_{d=1..n} K(d,2) * floor(n/d)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    int64_t sum = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        idx_t f = n / d;
        int k = 0;
        if ((d & 1ULL) == 0) {
            /* even d: K(d,2) = 0 */
            k = 0;
        } else {
            /* d odd: residu modulo 8 determines the Kronecker symbol */
            switch (d & 7ULL) {
                case 1: case 7: k =  1; break;
                case 3: case 5: k = -1; break;
                default:    k = 0;    /* unreachable, but kept for completeness */
            }
        }
        sum += (int64_t)k * (int64_t)f;
    }
    return (seq_t)sum;
}
