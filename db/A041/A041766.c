/*
 * A041766  Numerators of continued fraction convergents to sqrt(404).
 * https://oeis.org/A041766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 402*a(n-2) - a(n-4) Initial values: a(0)=20, a(1)=201,
 *            a(2)=8060, a(3)=80801
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return 20;
    if (n == 1) return 201;
    if (n == 2) return 8060;
    if (n == 3) return 80801;

    /* buffer holds the last four computed values:
       indices modulo 4: 0->a(n-3), 1->a(n-2), 2->a(n-1), 3->a(n)
    */
    seq_t buf[4] = {20, 201, 8060, 80801};

    /* constant coefficient cast to seq_t for safety with arbitrary-precision types */
    const seq_t coeff = (seq_t)402;

    /* compute values up to n */
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = coeff * a(i-2) - a(i-4) */
        buf[i % 4] = coeff * buf[(i - 2) % 4] - buf[(i - 4) % 4];
    }

    return buf[n % 4];
}
