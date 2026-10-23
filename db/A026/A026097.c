/*
 * A026097  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is
 *          an integer, s(0) = 0, |s(i) - s(i-1)| = 1 for i = 1,2,3; |s(i) -
 *          s(i-1)| <= 1 for i >= 4. Also a(n) = sum of numbers in row n+1 of
 *          the array T defined in A026082 and a(n) = 24*3^(n-4) for n >= 4.
 * https://oeis.org/A026097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24 * 3^(n-4) for n ≥ 4 a(0) = 1 a(1) = 2 a(2) = 4 a(3) =
 *            8 (the recurrence a(n) = 3·a(n-1) holds for n > 3)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the initial values explicitly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)4;
        case 3: return (seq_t)8;
        default:
            /* For n ≥ 4 compute 24 * 3^(n‑4) */
            return (seq_t)24 * pow3(n - 4);
    }
}
