/*
 * A119248  a(n) is the difference between denominator and numerator of the
 *          n-th alternating harmonic number Sum_{k=1..n} (-1)^(k+1)/k =
 *          A058313(n)/A058312(n).
 * https://oeis.org/A119248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = denominator(Sum_{k=1..n} (-1)^(k+1)/k) -
 *            numerator(Sum_{k=1..n} (-1)^(k+1)/k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but guard */

    seq_t num = 1;                   /* numerator of the alternating sum */
    seq_t den = 1;                   /* denominator of the alternating sum */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t product = num * (seq_t)i;   /* num * i */

        if (i & 1) {                   /* i odd -> + sign */
            /* new_num = num*i + den */
            seq_t new_num = product + den;
            seq_t new_den = den * (seq_t)i;
            seq_t g = gcd(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        } else {                      /* i even -> - sign */
            /* new_num = num*i - den (guaranteed non–negative) */
            seq_t new_num = product - den;
            seq_t new_den = den * (seq_t)i;
            seq_t g = gcd(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }
    }

    return den - num;                  /* denominator minus numerator */
}
