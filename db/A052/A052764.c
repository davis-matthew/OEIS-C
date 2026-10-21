/*
 * A052764  E.g.f.: -1/4*LambertW(-4*x).
 * https://oeis.org/A052764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n==0, otherwise a(n) = 2^(2*n-2) * n^(n-1) =
 *            (4*n)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* special case for the offset */
    if (n == 0) return 0;
    /* compute (4*n)^(n-1) using exponentiation by squaring */
    seq_t base = (seq_t)(4 * (seq_t)n);   /* convert factor first */
    idx_t  exp  = n - 1;
    seq_t result = 1;

    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
