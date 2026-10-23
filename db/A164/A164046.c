/*
 * A164046  A001445 written in base 2.
 * https://oeis.org/A164046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^n + 10^⌊(n-1)/2⌋
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Guard against calls below offset – behaviour undefined but
       we return 0 for convenience. */
    if (n < 1)
        return (seq_t)0;

    /* k = ⌊(n-1)/2⌋  using unsigned shift. */
    idx_t k = (n - 1) >> 1;

    /* Compute the two powers of 10 and add them. */
    seq_t term1 = pow10(n);
    seq_t term2 = pow10(k);
    return term1 + term2;
}
