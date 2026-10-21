/*
 * A093296  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 96.
 * https://oeis.org/A093296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10^(2n) - 64*10^n for n ≥ 5, special values: a(1)=0,
 *            a(2)=96, a(3)=5768, a(4)=662400. Implementation uses the
 *            provided pow10() utility
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Handle the first few terms explicitly. */
    if (n == 0) return 0;              /* offset starts at 1, but guard just in case */
    if (n == 1) return 0;
    if (n == 2) return 96;
    if (n == 3) return 5768;
    if (n == 4) return 662400;

    /* For n ≥ 5: a(n) = 10^(2n) - 64·10^n */
    idx_t two_n = n + n;                /* 2*n fits in idx_t since n≤10000 */
    seq_t ten_n   = pow10(n);
    seq_t ten_2n  = pow10(two_n);
    seq_t res = ten_2n - ( (seq_t)64 * ten_n );
    return res;
}
