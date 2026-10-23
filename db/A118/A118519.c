/*
 * A118519  Define sequence S_m by: initial term = m, reverse digits and add 3
 *          to get next term. Entry shows S_3. This reaches a cycle of length
 *          6 in 3 steps.
 * https://oeis.org/A118519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = rev(a(n‑1)) + 3, a(1) = 3
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)3;
    if (n <= 1)
        return val;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t rev = reverse_digits(val);
        val = rev + (seq_t)3;
    }
    return val;
}
