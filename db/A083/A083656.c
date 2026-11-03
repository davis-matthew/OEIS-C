/*
 * A083656  a(n) = Sum_{i=1..n} floor(r*floor(r*i)), where r=sqrt(2).
 * https://oeis.org/A083656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} floor(sqrt(2) * floor(sqrt(2) * i))
 * Range:     u8 n <= 8, u16 n <= 255, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 67 terms through n = 3973 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ii = (seq_t)i;

        /* floor(sqrt(2) * i)  ==  floor_sqrt_seq( 2 * i * i ) */
        seq_t t1 = isqrt_seq((seq_t)2 * ii * ii);

        /* floor(sqrt(2) * floor(sqrt(2) * i))
           == floor_sqrt_seq( 2 * t1 * t1 ) */
        seq_t t2 = isqrt_seq((seq_t)2 * t1 * t1);

        sum += t2;
    }
    return sum;
}
