/*
 * A069672  Largest n-digit triangular number with minimum digit sum.
 * https://oeis.org/A069672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1 a(2) = 10 a(3) = 300 a(4) = 3003 a(6) = 112101 a(8) =
 *            33020001 For odd n ≥ 5: a(n) = 2·10^(n−1) + 10^((n−1)/2) For
 *            even n ≥ 10: a(n) = 3·10^(n−1) + 2·10^(n−2) + 2·10^(n/2−1) +
 *            10^(n/2) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 30,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"     /* pow10(idx_t e) -> seq_t */
#include "../utils/mul_small.h" /* mul_small(seq_t x, unsigned int m) -> seq_t */

seq_t a(idx_t n)
{
    /* Handle the special short cases first */
    if (n == 1) return 1;
    if (n == 2) return 10;
    if (n == 3) return 300;
    if (n == 4) return 3003;
    if (n == 6) return 112101;
    if (n == 8) return 33020001;

    /* For odd n ≥ 5 */
    if (n & 1) {
        seq_t term1 = mul_small(pow10(n-1), 2);
        seq_t term2 = pow10((n-1)/2);
        return term1 + term2;
    }

    /* For even n ≥ 10 */
    seq_t res = mul_small(pow10(n-1), 3);                 /* 3·10^(n−1) */
    res += mul_small(pow10(n-2), 2);                      /* +2·10^(n−2) */
    res += mul_small(pow10(n/2 - 1), 2);                   /* +2·10^(n/2−1) */
    res += pow10(n/2);                                    /* +10^(n/2)   */
    res += 1;                                             /* +1           */
    return res;
}
