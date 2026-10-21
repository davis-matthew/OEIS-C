/*
 * A085404  Number of n X n matrices over GF(4) with rank n-1.
 * https://oeis.org/A085404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{j=0}^{n-2} (4^n - 4^j)^2 / (4^{n-1} - 4^j)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n = 1, the empty product is 1 */
    if (n <= 1)
        return (seq_t)1;

    /* 4^n and 4^{n-1} */
    seq_t pow4n   = big_pow((seq_t)4, (seq_t)n);
    seq_t pow4nm1 = pow4n / ((seq_t)4);   /* exact division */

    seq_t power_j = (seq_t)1;            /* 4^j, starting with j = 0 */

    seq_t num = (seq_t)1;   /* numerator of the rational product */
    seq_t den = (seq_t)1;   /* denominator */

    for (idx_t j = 0; j <= n - 2; ++j)
    {
        seq_t diff_n   = pow4n   - power_j;
        seq_t diff_nm1 = pow4nm1 - power_j;

        /* Multiply numerator by (4^n - 4^j) twice, reducing with the
           current denominator factor each time. */
        num = num * diff_n;
        seq_t g = gcd(num, diff_nm1);
        if (g != (seq_t)1) {
            num   = num   / g;
            diff_nm1 = diff_nm1 / g;
        }

        num = num * diff_n;
        g = gcd(num, diff_nm1);
        if (g != (seq_t)1) {
            num   = num   / g;
            diff_nm1 = diff_nm1 / g;
        }

        /* Multiply accumulated denominator by the remaining part */
        den = den * diff_nm1;

        /* Reduce overall fraction to keep numbers small */
        g = gcd(num, den);
        if (g != (seq_t)1) {
            num = num / g;
            den = den / g;
        }

        /* Advance 4^j to 4^{j+1} */
        power_j = power_j * ((seq_t)4);
    }

    /* The result is an integer, so den should be 1 */
    return num / den;
}
