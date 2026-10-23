/*
 * A177002  Period 4: repeat [1, 2, 4, 2].
 * https://oeis.org/A177002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) repeats the 4‑term pattern [1, 2, 4, 2] for all n ≥ 0
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pat[4] = {1, 2, 4, 2};
    return pat[(size_t)(n % 4)];
}
