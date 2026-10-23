/*
 * A121544  Sum of all proper base 4 numbers with n digits (those not
 *          beginning with 0).
 * https://oeis.org/A121544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 3 * 4^(n-1) * (4^(n-1) + 4^n - 1) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1; behaviour below 1 is undefined */
    idx_t exp = n - 1;                  /* exponent for the base 4 power */
    seq_t k = pow4(exp);                /* k = 4^(n-1) */
    seq_t t = (seq_t)5 * k - 1;         /* t = 5*k - 1  (= 4^n + 4^(n-1) - 1) */

    /* Divide the product by 2 in a way that keeps intermediate values
       inside the numeric range of seq_t (for 64‑bit builds).  Either t
       or k is even depending on the parity of k. */
    if ((t & 1) == 0) {                 /* t is even */
        t >>= 1;                        /* t = t / 2 */
        return (seq_t)3 * k * t;        /* 3 * k * t */
    } else {                            /* k is even, t is odd */
        seq_t k2 = k >> 1;              /* k2 = k / 2 */
        return (seq_t)3 * k2 * t;       /* 3 * k2 * t */
    }
}
