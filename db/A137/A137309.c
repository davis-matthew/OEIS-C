/*
 * A137309  Let T = {1,3,5,7,9,1,3,5,7,9,1,3,5,7,9, ... }; a(n) is n-th
 *          concatenation of n numbers from T.
 * https://oeis.org/A137309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenation of the next n digits of the repeating
 *            pattern 1,3,5,7,9, starting after (n−1)n/2 previous digits of
 *            the same pattern
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* precompute the cyclic list of digits */
    static const unsigned char odd[5] = {1, 3, 5, 7, 9};

    /* distance to the start of the n‑th block */
    idx_t start_idx = ((n - 1) * n) / 2;          /* triangular number (n−1)n/2 */
    idx_t start_pos = start_idx % 5;              /* position in the 5‑cycle */

    seq_t val = 0;                                /* result accumulator */

    for (idx_t i = 0; i < n; ++i) {
        unsigned char d = odd[(start_pos + i) % 5];
        val = val * 10 + d;                       /* shift left one decimal place and add digit */
    }

    return val;
}
