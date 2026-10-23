/*
 * A184750  a(n) = largest k such that A000326(n+1) = A000326(n) + (A000326(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A184750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n ≤ 4; a(n) = (3n^2 – 7n – 2)/2 for n ≥ 5
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 4)
        return 0;

    seq_t sn = (seq_t)n;
    seq_t numerator = 3 * sn * sn - 7 * sn - 2;
    return numerator / 2;
}
