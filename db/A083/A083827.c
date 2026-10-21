/*
 * A083827  Let x(n) = 123...n, y(n) = n...321; a(n) = y(n) concatenated with
 *          x(n) - x(n) concatenated with y(n).
 * https://oeis.org/A083827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = (y(n) - x(n)) * (10^len(x(n)) - 1) where x(n) =
 *            concatenation of 1 … n and y(n) = reverse of x(n)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   digits_of, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/digits_of.h"

 seq_t a(idx_t n)
{
    /* pre‑compute powers of 10 up to the largest possible digit count */
    static bool init = false;
    static seq_t pow10[21];          /* pow10[d] = 10^d for d = 0..20 */
    if (!init) {
        pow10[0] = 1;
        for (int d = 1; d <= 20; ++d)
            pow10[d] = pow_uint(10, d);
        init = true;
    }

    seq_t x = 0;
    seq_t y = 0;
    idx_t len = 0;                 /* number of decimal digits of x(n) */

    /* build x(n) and count its digits */
    for (idx_t k = 1; k <= n; ++k) {
        int d = digits_of(k);       /* digits of k (1 ≤ k ≤ n) */
        len += (idx_t)d;
        x = x * pow10[d] + (seq_t)k;
    }

    /* build y(n) (reverse concatenation) */
    for (idx_t k = n; k >= 1; --k) {
        int d = digits_of(k);
        y = y * pow10[d] + (seq_t)k;
    }

    seq_t diff   = y - x;
    seq_t powL   = pow_uint(10, len);      /* 10^len */
    seq_t factor = powL - (seq_t)1;        /* 10^len - 1 */

    return diff * factor;
}
