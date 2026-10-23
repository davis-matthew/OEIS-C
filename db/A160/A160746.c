/*
 * A160746  a(n) = A151566(n)*4.
 * https://oeis.org/A160746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * A151566(n) A151566(0) = 0 A151566(1) = 1 A151566(2) =
 *            2 For n ≥ 2: if n is a power of two (n = 2 k, k ≥ 2) A151566(n)
 *            = 3 · A151566(n/2) else let m = 2⌊log₂ n⌋ (the largest power of
 *            two < n) and r = n – m A151566(n) = A151566(m) + 2 · A151566(r)
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   A151566
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A151566.h"

seq_t a(idx_t n)
{
    return (seq_t)4 * A151566(n);
}
