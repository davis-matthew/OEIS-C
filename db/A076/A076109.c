/*
 * A076109  Least positive k such that k^n is the sum of n consecutive
 *          integers, or 0 if no such k exists.
 * https://oeis.org/A076109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 0 if n ≡ 0 (mod 4); a(n) = rad(n) if n odd; a(n) =
 *            rad(n/2) if n ≡ 2 (mod 4), where rad(m) is the product of
 *            distinct prime factors of m
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8840 (2026-11-01)
 * Helpers:   radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/radical.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n % 4 == 0) return (seq_t)0;
    if ((n & 1ULL) == 1)          /* odd n */
        return radical(n);
    return radical(n >> 1);           /* n ≡ 2 (mod 4) */
}
