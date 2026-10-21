/*
 * A070366  a(n) = 5^n mod 9.
 * https://oeis.org/A070366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 9
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[6] = { (seq_t)1, (seq_t)5, (seq_t)7,
                                     (seq_t)8, (seq_t)4, (seq_t)2 };
    return period[(size_t)(n % 6)];
}
