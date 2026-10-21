/*
 * A055350  Number of labeled mobiles (circular rooted trees) with n nodes and
 *          3 leaves.
 * https://oeis.org/A055350
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(n-1)*(n-2)*(n-3)*(3*n-4)/144, OEIS A055350, offset 4
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;           /* values below the offset are undefined */

    idx_t i;
    seq_t denom = 144;
    seq_t result = 1;

    /* Multiply by factorial 1..n, cancelling with denominator where possible. */
    for (i = 1; i <= n; ++i) {
        seq_t f = (seq_t)i;

        seq_t g = gcd_seq_t(result, denom);
        if (g > (seq_t)1) {
            result /= g;
            denom /= g;
        }

        g = gcd_seq_t(f, denom);
        if (g > (seq_t)1) {
            f /= g;
            denom /= g;
        }

        result *= f;
    }

    /* Multiply the remaining factors (n-1)*(n-2)*(n-3)*(3n-4). */
    seq_t extras[4];
    extras[0] = (seq_t)(n - 1);
    extras[1] = (seq_t)(n - 2);
    extras[2] = (seq_t)(n - 3);
    extras[3] = (seq_t)(3 * n - 4);

    for (i = 0; i < 4; ++i) {
        seq_t f = extras[i];

        seq_t g = gcd_seq_t(result, denom);
        if (g > (seq_t)1) {
            result /= g;
            denom /= g;
        }

        g = gcd_seq_t(f, denom);
        if (g > (seq_t)1) {
            f /= g;
            denom /= g;
        }

        result *= f;
    }

    /* Final division by any remaining part of the denominator. */
    if (denom != (seq_t)1)
        result /= denom;

    return result;
}
