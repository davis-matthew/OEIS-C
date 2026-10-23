/*
 * A190727  Product of (digits of n each incremented by 1) - 2.
 * https://oeis.org/A190727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{digits d of n} (d + 1) – 2 (n ≥ 1)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1, but guard against 0 */
        return 0;

    seq_t prod = 1;
    idx_t x = n;

    do {
        /* multiply by (digit + 1) */
        prod = prod * (seq_t)((x % 10) + 1);
        x /= 10;
    } while (x > 0);

    /* subtract 2 to exclude the numbers 0 and n themselves */
    return prod - 2;
}
