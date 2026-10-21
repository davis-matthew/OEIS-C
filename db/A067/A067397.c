/*
 * A067397  Maximal power of 3 that divides n-th Catalan number.
 * https://oeis.org/A067397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A067397 – Maximal power of 3 that divides the n‑th Catalan number.
 * Recurrence (for n ≥ 0):
 *   a(0) = 0.  Let k = ⌊log₃ n⌋,  base = 3ᵏ.
 *   If n == 3·base – 1             → a(n) = 0.
 *   Else if  (3/2)·base < n < (5/2)·base
 *        → a(n) = a(n – base) + 1.
 *   Else                            → a(n) = a(n – base).
 */

#include <stdint.h>
#include "../utils/a_rec.h"

seq_t a(idx_t n)
{
    return a_rec(n);
}
