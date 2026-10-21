/*
 * A052796  Expansion of e.g.f. x^4*exp(x)^2.
 * https://oeis.org/A052796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n < 4 a(n) = (n-3)*(n-2)*(n-1)*n * 2^(n-4) for n ≥
 *            4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 19, u64 n <= 46, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 4u)
        return 0;

    /* compute (n-3)*(n-2)*(n-1)*n  */
    seq_t prod = (seq_t)(n - 3u) *
                 (seq_t)(n - 2u) *
                 (seq_t)(n - 1u) *
                 (seq_t)n;

    /* compute 2^(n-4) by repeated doubling */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 4u; ++i)
        pow2 += pow2;   /* multiply by 2 */

    return prod * pow2;
}
