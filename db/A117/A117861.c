/*
 * A117861  Number of palindromes of length n (in base 9).
 * https://oeis.org/A117861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*9^⌊(n-1)/2⌋
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* not defined for n<1, return 0 */
    idx_t k = (n - 1) / 2;                /* floor((n-1)/2) */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)k);
    return (seq_t)8 * pow9;
}
