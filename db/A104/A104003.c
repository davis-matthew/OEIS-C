/*
 * A104003  (3/4)*(27*n^2-137*n+180)*n^(n-6)*(2*n-6)!/(n-3)!.
 * https://oeis.org/A104003
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/4)*(27*n^2-137*n+180)*n^(n-6)*(2*n-6)!/(n-3)!
 * Range:     u8 none, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast power: computes base^exp for non‑negative exp */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)          /* offset 4 – values below are irrelevant */
        return 0;

    /* coefficient part: (27*n^2 – 137*n + 180) * 3 */
    seq_t coeff = (seq_t)27 * (seq_t)n * (seq_t)n
                - (seq_t)137 * (seq_t)n
                + (seq_t)180;
    seq_t temp = coeff * (seq_t)3;

    /* product of k from n-2 to 2n-6 */
    seq_t prod = 1;
    for (idx_t k = n - 2; k <= 2 * n - 6; ++k)
        prod *= (seq_t)k;

    temp *= prod;           /* now temp = 3*coeff*Πk */

    if (n >= 6) {
        /* positive exponent: multiply by n^(n-6) */
        seq_t pow_fac = pow_seq((seq_t)n, n - 6);
        temp *= pow_fac;
    } else {
        /* negative exponent: divide by n^(6-n) */
        seq_t denom = pow_seq((seq_t)n, 6 - n);
        temp /= denom;
    }

    /* final division by 4 */
    temp /= 4;
    return temp;
}
