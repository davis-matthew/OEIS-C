/*
 * A172498  a(n) = denominator of fraction a/b, where gcd(a, b) = 1, whose
 *          decimal representation has the form 0.(1)(2)(3)...(n-1)(n)... with
 *          period (1)(2)(3)...(n-1)(n).
 * https://oeis.org/A172498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (10^k - 1)/gcd(C, 10^k – 1), where C is the integer
 *            formed by concatenating 1,2,…,n and k is the total number of
 *            decimal digits of C
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   digits, pow10, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/seq_gcd.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    seq_t C = 0;          /* concatenated number */
    idx_t  k = 0;         /* total digit count */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t d = digits(i);          /* digits of i */
        k += d;
        C = C * pow10(d) + i;        /* append i to C */
    }

    seq_t ten_pow_k = pow10(k);     /* 10^k   */
    seq_t den       = ten_pow_k - 1;/* 10^k - 1 */
    seq_t g         = seq_gcd(C, den);

    return den / g;                 /* reduced denominator */
}
