/*
 * A063915  Expansion of g.f. (1 + Sum_{ i >= 0 } 2^i*x^(2^(i+1)-1)) /
 *          (1-x)^2.
 * https://oeis.org/A063915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (n+2)*2^k - (2*4^k + 1)/3 where k = floor(log2(n+2))
 * Range:     u8 n <= 13, u16 n <= 423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes 2^exp using repeated left shift.  Unsigned shift is safe for
   exp <= 63; larger values would overflow the builtin type and are not
   needed because the resulting a(n) would not fit in seq_t for the test
   cases. */
static seq_t power_of_two(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res <<= 1;
    return res;
}

seq_t a(idx_t n)
{
    idx_t np2 = n + 2;      /* n+2 is at least 1 */

    /* Compute k = floor(log2(n+2)) by counting shifts */
    idx_t k = 0;
    while (np2 > 1) {
        np2 >>= 1;
        ++k;
    }

    /* 2^k and 4^k = (2^k)^2 */
    seq_t pow2k = power_of_two(k);
    seq_t pow4k = pow2k * pow2k;   /* 2^(2k) */

    /* term = (n+2) * 2^k */
    seq_t term = ((seq_t)(n + 2)) * pow2k;

    /* subtract = (2*4^k + 1)/3 */
    seq_t subtract = ((pow4k << 1) + 1) / 3;

    return term - subtract;
}
