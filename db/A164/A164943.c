/*
 * A164943  Decimal value of the concatenation of first n odd numbers in
 *          binary.
 * https://oeis.org/A164943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 373 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = decimal value of the binary concatenation of the first n odd numbers. */

#include <stdint.h>   /* seq_t and idx_t are provided externally */
#include "../utils/bit_length.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t odd = ((seq_t)k << 1) - 1;           /* 2k-1 */
        unsigned int w = bit_length(odd);      /* bit‑length of odd */
        result = (result << (unsigned long)w) + odd;
    }
    return result;
}
