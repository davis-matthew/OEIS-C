/*
 * A124812  Number of 4-ary Lyndon words of length n with exactly four 1s.
 * https://oeis.org/A124812
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*C(n-1,3)*3^(n-4) for odd n a(n) =
 *            (1/4)*(C(n-1,3)*3^(n-4) - (n/2-1)*3^((n-4)/2)) for even n where
 *            C(a,b) is the binomial coefficient
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Sequence defined for n ≥ 5; behavior for lower n is irrelevant. */
    if (n < 5) return 0;

    seq_t C = binom(n - 1, 3);           /* C = (n-1 choose 3) */

    if (n & 1) {                         /* odd n */
        seq_t pow_val = pow3(n - 4);     /* 3^(n-4) */
        /* C is divisible by 4 for odd n, so division is exact. */
        return (C / 4) * pow_val;
    } else {                             /* even n */
        seq_t pow_big  = pow3(n - 4);    /* 3^(n-4)          */
        seq_t pow_half = pow3((n - 4) / 2); /* 3^((n-4)/2)    */
        seq_t term1 = C * pow_big;
        seq_t term2 = (seq_t)(n / 2 - 1) * pow_half;
        return (term1 - term2) / 4;
    }
}
