/*
 * A144743  Recurrence sequence a(n)=a(n-1)^2-a(n-1)-1, a(0)=3.
 * https://oeis.org/A144743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 - a(n-1) - 1, a(0) = 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>        /* for uintptr_t, if needed */
seq_t a(idx_t n)
{
    /* The sequence grows extremely fast; each term is computed
       directly from the previous one.  The algorithm is O(n)
       and works for any seq_t that supports the arithmetic
       operators +, -, *, and / (e.g. uint*_t or an arbitrary
       precision integer class in C++). */
    if (n == 0)
        return (seq_t)3;

    seq_t x = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i)
        x = x * x - x - 1;
    return x;
}
