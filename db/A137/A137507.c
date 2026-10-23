/*
 * A137507  a(n) = 100^[n/10] + 2*n*10^[n/10]: inspired by Engel expansion of
 *          Pi.
 * https://oeis.org/A137507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100^[n/10] + 2*n*10^[n/10]
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 49, u64 n <= 99, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* k = floor(n / 10) */
    idx_t k = n / 10;

    /* Compute 10^k */
    seq_t pow10k = 1;
    for (idx_t i = 0; i < k; ++i)
    {
        pow10k *= 10;
    }

    /* Compute 100^k = (10^k)^2 */
    seq_t base100k = pow10k * pow10k;

    /* Compute 2 * n * 10^k */
    seq_t term2 = (seq_t)n * 2 * pow10k;

    /* Result */
    return base100k + term2;
}
