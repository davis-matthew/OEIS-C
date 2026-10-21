/*
 * A081912  a(n) = 6^n*(n^2 - n + 72)/72.
 * https://oeis.org/A081912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n*(n^2 - n + 72)/72 This implementation avoids overflow
 *            for 64‑bit builds by factoring the division of 72 into the power
 *            of 6 before multiplication
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n as seq_t for polynomial arithmetic */
    seq_t ns = (seq_t)n;

    /* Compute the polynomial part n^2 - n + 72 */
    seq_t poly = ns * ns - ns + (seq_t)72;

    if (n < 3) {
        /* Small n: compute directly, division by 72 is safe */
        seq_t pow6 = seq_pow((seq_t)6, n);
        return pow6 * poly / (seq_t)72;
    } else {
        /* For n >= 3, 6^n / 72 = 2^(n-3) * 3^(n-2) is an integer */
        seq_t two_pow = seq_pow((seq_t)2, n - 3);
        seq_t three_pow = seq_pow((seq_t)3, n - 2);
        seq_t base = two_pow * three_pow;          /* 2^(n-3) * 3^(n-2) */
        return base * poly;                      /* times (n^2 - n + 72) */
    }
}
