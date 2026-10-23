/*
 * A138141  Numbers with digits in ascending order that differ exactly by 1.
 * https://oeis.org/A138141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with digits in ascending order that differ exactly by 1. */
/* The function computes a(n) by determining the length k of the number
 * and its starting digit, then concatenating the consecutive digits. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cumulative counts of terms up to length k (1 ≤ k ≤ 9) */
    static const idx_t cum[10] = {0, 9, 17, 24, 30, 35, 39, 42, 44, 45};

    if (n == 0 || n > cum[9])
        return 0;                     /* out of range: no such term */

    idx_t k = 1;
    while (k <= 9 && n > cum[k])
        ++k;                          /* find the length k with cum[k-1] < n ≤ cum[k] */

    idx_t offset = n - cum[k - 1] - 1; /* offset within the group of length k */
    seq_t value = 0;
    seq_t start = (seq_t)(1 + offset);/* starting digit */

    for (idx_t i = 0; i < k; ++i)
        value = value * 10 + (start + i);

    return value;
}
