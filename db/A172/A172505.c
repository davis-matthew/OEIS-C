/*
 * A172505  a(n) = numerator of fraction a/b, where gcd(a, b) = 1, whose
 *          decimal representation has the form
 *          (1)(2)(3)...(n-1)(n).(1)(2)(3)...(n-1)(n)... with period
 *          (1)(2)(3)...(n-1)(n).
 * https://oeis.org/A172505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = N*10^k / gcd(N,10^k-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   gcd, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where N is the integer obtained by concatenating the decimal
   representations of 1,2,…,n and k = number of digits of N. */
#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t N = 0;              /* concatenated number */
    idx_t  sum_digits = 0;    /* k = total number of digits of N */

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t d = num_digits0(i);           /* digits of i   */
        sum_digits += d;                    /* accumulate k  */
        seq_t factor = pow10(d);             /* 10^d          */
        N = N * factor + (seq_t)i;          /* append i      */
    }

    seq_t pow10_k = pow10(sum_digits);          /* 10^k          */
    seq_t denom   = pow10_k - 1;                /* 10^k - 1      */
    seq_t g       = gcd(N, denom);              /* gcd(N,10^k-1) */
    seq_t result  = (N * pow10_k) / g;          /* simplified numerator */
    return result;
}
