/*
 * A018288  Divisors of 110.
 * https://oeis.org/A018288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 110 (sorted ascending)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    enum { Count = 8 };
    static const seq_t divisors[Count] = {1, 2, 5, 10, 11, 22, 55, 110};
    if (n > (idx_t)Count) return 0;
    return divisors[(size_t)(n - 1)];
}
