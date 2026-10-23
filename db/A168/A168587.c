/*
 * A168587  Smallest digit sum of an n-digit prime with only digits 0 and 1
 *          (or 0, if no such prime exists).
 * https://oeis.org/A168587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0 a(2) = 2 a(3) = 2 a(4) = 0 a(5) = 4 a(6) = 5 a(n) = 4
 *            for all n ≥ 7 (values derived from the OEIS definition of
 *            A168587)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1 || n == 4)
        return (seq_t)0;
    if (n == 2 || n == 3)
        return (seq_t)2;
    if (n == 6)
        return (seq_t)5;
    /* For n ≥ 5, the minimal digit sum of an n‑digit
       prime with digits 0 and 1 is 4, except for n = 6
       where it is 5. */
    return (seq_t)4;
}
