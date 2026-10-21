/*
 * A083458  Numbers n such that n^5 + 1 is a palindrome.
 * https://oeis.org/A083458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083458: numbers n such that n^5 + 1 is a palindrome.
   Formula: a(1) = 1, a(2) = 2, a(3) = 6, a(n) = 10^(n-3) for n ≥ 4. */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;
    /* For n ≥ 4, the terms are powers of ten: 10, 100, 1000, ... */
    return pow10(n - 3);
}
