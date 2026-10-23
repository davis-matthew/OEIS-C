/*
 * A115545  Numbers k such that the concatenation of 6*k with k gives a
 *          square.
 * https://oeis.org/A115545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For odd n = 2m-1 → a(n) = (6*10^d + 1)/49 , with d = 42*m + 6.
 *            For even n → a(n) = 4 * a(n-1). (The formula comes from the fact
 *            that 6*10^d + 1 is divisible by 49 exactly when d ≡ 6 (mod 42),
 *            which yields the shown pattern.)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; behaviour for n==0 is undefined, return 0 */
        return (seq_t)0;

    if ((n & 1) == 1) {                 /* odd index */
        /* m = (n-1)/2 is the count of completed odd blocks */
        idx_t m = (n - 1) / 2;
        idx_t d = 42 * m + 6;           /* degree of power of ten */
        seq_t p10 = pow10(d);           /* 10^d, exact */
        seq_t a_val = 6 * p10 + 1;      /* (6*10^d + 1) */
        seq_t k = a_val / 49;           /* guaranteed integer division */
        return k;
    } else {                            /* even index */
        /* a(2m) = 4 * a(2m-1) */
        return 4 * a(n - 1);
    }
}
