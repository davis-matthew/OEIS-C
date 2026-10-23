/*
 * A040645  Continued fraction for sqrt(671).
 * https://oeis.org/A040645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=floor(sqrt(671)); for n>0 a(n) repeats the 10–term block
 *            [1,9,2,1,1,1,2,9,1,50]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)25;
    }
    static const seq_t period[10] = {
        (seq_t)1, (seq_t)9, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)2, (seq_t)9, (seq_t)1, (seq_t)50
    };
    size_t idx = (size_t)((n - 1) % 10);
    return period[idx];
}
