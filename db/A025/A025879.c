/*
 * A025879  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A025879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+19)*(n+26)/600 + (n+22)*(-1)^n/120 - (n+8)*(n
 *            mod 5)/50) Computes the number of partitions of n into parts 5,
 *            6, and 10
 * Range:     u8 n <= 369, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use signed 64‑bit arithmetic for the intermediate calculation */
    int64_t n64 = (int64_t)n;

    /* (n+19)*(n+26) */
    int64_t part1 = (n64 + 19) * (n64 + 26);

    /* (n+22)*(-1)^n  --> sign depends on parity of n */
    int64_t part2 = (n64 + 22) * ((n64 & 1) ? -1 : 1);

    /* (n+8)*(n mod 5) */
    int64_t part3 = (n64 + 8) * (n64 % 5);

    /* common denominator 600:  (part1 + 5*part2 - 12*part3) / 600 */
    int64_t numerator = part1 + 5 * part2 - 12 * part3;

    /* numerator is always non‑negative for n ≥ 0, so simple truncation works */
    return (seq_t)(numerator / 600);
}
