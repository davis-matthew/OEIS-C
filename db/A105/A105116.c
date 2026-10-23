/*
 * A105116  The part of n left of the decimal point when written in base e
 *          using a greedy algorithm representation.
 * https://oeis.org/A105116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer part of the greedy base‑e representation of
 *            n, written as a decimal number whose digits are 0,1,2
 * Range:     u8 n <= 20, u16 n <= 148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* e will be computed only once; it is a long double (≈80‑bit) */
    static long double e = 0.0L;
    if (e == 0.0L) e = expl(1.0L);

    long double nld = (long double)n;

    /* Find the highest power k such that e^k <= n < e^(k+1) */
    idx_t k = 0;
    long double cur = 1.0L;                     /* cur = e^k   */
    while (cur <= (nld / e)) {                  /* test e^(k+1) <= n  */
        cur *= e;
        ++k;
    }

    /* Build the integer part digit by digit, most significant first */
    seq_t res = 0;
    long double rem = nld;
    for (idx_t i = k; ; --i) {
        int digit = (int)(rem / cur);           /* 0 <= digit <= 2  */
        res = res * 10 + (seq_t)digit;          /* append digit in decimal */
        rem -= digit * cur;
        if (i == 0) break;
        cur /= e;
    }
    return res;
}
