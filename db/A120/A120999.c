/*
 * A120999  Denominators of partial sums of Catalan numbers scaled by powers
 *          of 1/7^2 = 1/49.
 * https://oeis.org/A120999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the partial sum r(n) = sum_{k=0}^n
 *            Catalan(k) / 49^k, expressed in lowest terms
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* r(0) = 1/1 */

    seq_t num = 1;                  /* numerator of the current sum   */
    idx_t exp = 0;                  /* exponent of 7 in the denominator (den = 7^exp) */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t dexp = 2 * k;                     /* denominator exponent of the k‑th term */
        seq_t term = catalan(k);                /* C_k                               */
        seq_t new_num;
        idx_t new_exp;

        if (exp <= dexp) {                     /* common denominator 7^dexp         */
            seq_t mul = 1;
            for (idx_t i = 0; i < dexp - exp; ++i) mul *= (seq_t)7;
            new_num = num * mul + term;
            new_exp = dexp;
        } else {                               /* common denominator 7^exp          */
            seq_t mul = 1;
            for (idx_t i = 0; i < exp - dexp; ++i) mul *= (seq_t)7;
            new_num = num + term * mul;
            new_exp = exp;
        }

        /* remove factors of 7 that divide the new numerator */
        idx_t t = 0;
        while (new_num % (seq_t)7 == 0) {
            new_num /= (seq_t)7;
            ++t;
        }

        exp = new_exp - t;
        num = new_num;
    }

    /* reconstruct the denominator as 7^exp */
    seq_t den = 1;
    for (idx_t i = 0; i < exp; ++i) den *= (seq_t)7;
    return den;
}
