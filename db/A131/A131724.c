/*
 * A131724  Period 6: repeat [1, 9, 7, 13, 11, 9].
 * https://oeis.org/A131724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) for n > 5, i.e., the sequence repeats the period
 *            [1, 9, 7, 13, 11, 9]. This implementation works for any seq_t
 *            (fixed‑size or arbitrary‑precision) and any idx_t (uint64_t)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    static const seq_t period[6] = { 1, 9, 7, 13, 11, 9 };
    return period[(size_t)(n % 6)];
}
