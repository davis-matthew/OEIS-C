/*
 * A225668  a(n) = floor(4*log_2(n)).
 * https://oeis.org/A225668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(4*log_2(n))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   bit_length, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/ipow_idx.h"

/* Return the n‑th term of OEIS A225668.
 * The computation is performed as a(n) = floor(log_2(n^4))
 * which equals the number of bits of n^4 minus one.          */
seq_t a(idx_t n)
{
    /* n^4 (performed in the arbitrary‑precision type seq_t) */
    seq_t val = ipow_idx((seq_t)n, 4);

    /* floor(log_2(val)) = bitlen(val) – 1 */
    return (seq_t)(bit_length(val) - 1);
}
