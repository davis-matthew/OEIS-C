/*
 * A046237  Numbers whose cube is palindromic in base 7.
 * https://oeis.org/A046237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0, a(2)=1, a(3)=2, a(4)=4, a(5)=8, a(6)=16. For n≥7 let
 *            k=n−7; then a(n) = (1 << (k mod 3)) * (7^(⌊k/3⌋+2) + 1). The
 *            sequence consists of the numbers d*(7^m+1) for d=1,2,4 (in that
 *            order) and m=2,3,4,
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    static const seq_t pre[6] = {0ULL, 1ULL, 2ULL, 4ULL, 8ULL, 16ULL};

    if (n <= 6) return pre[n - 1];

    idx_t k = n - 7;
    idx_t m = k / 3 + 2;      /* exponent of 7 */
    idx_t r = k % 3;          /* 0→1, 1→2, 2→4 */
    seq_t d = (seq_t)1 << r;  /* 1,2,4 */

    seq_t pow7 = big_pow((seq_t)7, (seq_t)m);
    return d * (pow7 + 1);
}
