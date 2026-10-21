/*
 * A078258  a(n) = numerator(N), where N = 0.123...n (concatenation of 1 to n
 *          after decimal point).
 * https://oeis.org/A078258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, frac
 * Formula:   a(n) = numerator(0.123…n) a(n) is obtained by reducing the
 *            fraction S(n)/10^D(n) where S(n) is the integer obtained by
 *            concatenating 1,2,…,n and D(n) is the number of digits in S(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   pow10, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    seq_t S      = 0;          /* concatenated integer */
    idx_t D      = 0;          /* total number of digits */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute number of decimal digits of k without helpers */
        idx_t temp = k;
        int dk = 1;
        while (temp >= 10) {
            temp /= 10;
            ++dk;
        }

        seq_t mult = pow10((idx_t)dk);   /* 10^dk  */
        S = S * mult + (seq_t)k;        /* shift left and append k   */
        D += dk;
    }

    seq_t denom = pow10(D);               /* 10^D   */
    seq_t g     = seq_gcd(S, denom);      /* gcd(S,10^D)   */
    return S / g;                         /* numerator after reduction   */
}
