/*
 * A109733  Sequence is its own base-10 ASCII representation.
 * https://oeis.org/A109733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (if valuation(n/2^v + 1, 2) mod 5 = 3 then 56 else 57) -
 *            2*((v-3) mod 5), where v = valuation(n,2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   trailing_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/trailing_zeros.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1. No checks on n==0 as per specification. */
    idx_t v = trailing_zeros(n);
    idx_t m = n >> v;                 /* m is odd */
    idx_t w = trailing_zeros(m + 1);  /* valuation of (m+1) at 2 */
    idx_t w_mod5 = w % 5;
    seq_t base = (w_mod5 == 3) ? 56 : 57;

    /* Compute (v-3) mod 5 safely using (v+2)%5. */
    idx_t temp = (v + 2) % 5;
    return base - (seq_t)(2 * temp);
}
