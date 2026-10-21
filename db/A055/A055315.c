/*
 * A055315  Number of labeled trees with n nodes and 3 leaves.
 * https://oeis.org/A055315
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n-3)*(n-2)/12 Equivalent iterative recurrence: a(4) =
 *            4 a(n) = a(n-1) * n * (n-2) / (n-4) for n >= 5
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers.
   Not colliding with any existing helper name. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;              /* below offset, behaviour irrelevant */

    /* a(4) = 4 */
    seq_t res = 4U;

    for (idx_t i = 5; i <= n; ++i) {
        /* compute res = res * i * (i-2) / (i-4) with early cancellation */
        uint64_t num = i * (i - 2);   /* n*(n-2)   */
        uint64_t den = i - 4;         /* n-4       */

        /* cancel common factors between numerator and denominator */
        uint64_t g = gcd_u64(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* cancel common factors between current result and remaining denominator */
        seq_t rem = res % (seq_t)den;
        uint64_t g2 = gcd_u64((uint64_t)rem, den);
        if (g2 > 1) {
            res /= (seq_t)g2;
            den /= g2;
        }

        /* at this point den should be 1.
           But in the 64‑bit build the product may overflow,
           which is acceptable for values that do not fit. */
        res = res * (seq_t)num;
        if (den > 1)
            res /= (seq_t)den;
    }

    return res;
}
