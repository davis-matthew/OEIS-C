/*
 * A006230  Bitriangular permutations.
 * https://oeis.org/A006230
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 - 6*2^(n-2) + 2*3^(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   a000248_pow, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"
#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t pow2 = pw2(n - 2);
    seq_t pow3 = a000248_pow((seq_t)3, n - 2);
    return (seq_t)7 + (seq_t)2 * pow3 - (seq_t)6 * pow2;
}
