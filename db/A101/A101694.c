/*
 * A101694  A101693(2^n).
 * https://oeis.org/A101694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * ∏_{i=1}^{n-1} (2^{2^i} + 1) + 1, with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t product = 1;

    /* Compute the product of Fermat numbers up to 2^(2^(n-1)) + 1 */
    for (idx_t i = 1; i < n; ++i) {
        /* Compute f = 2^{2^i} + 1 by repeated squaring */
        seq_t f = 2;
        for (idx_t j = 0; j < i; ++j)
            f *= f;   /* after i steps f = 2^{2^i} */
        f += 1;      /* f = 2^{2^i} + 1 (Fermat number) */

        product *= f;
    }

    return product * 4 + 1;
}
