/*
 * A217589  Bit reversed 16-bit numbers.
 * https://oeis.org/A217589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = bit_reverse(n + 65536) >> 1 where bit_reverse(x) reverses
 *            the bits of x up to the most significant 1. For n in [0,65535]
 *            this is the usual 16‑bit bit‑reversal, and the formula extends
 *            the mapping to all n ≥ 0
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bit_reverse_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_reverse_seq_t.h"

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n + (seq_t)65536;  /* n + 2^16 */
    seq_t rev = bit_reverse_seq_t(x);      /* reverse bits */
    return rev >> 1;                       /* divide by 2 */
}
