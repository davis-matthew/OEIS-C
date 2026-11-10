/*
 * A065270  Inverse permutation to A065269.
 * https://oeis.org/A065270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   inverse of A065269. a(1)=1; for n in binary: "11..0" -> n/2;
 *            "10..." -> 2n+1; "11..1" -> (10 followed by complement of the
 *            bits after the leading "11")
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    if (m == 1)
        return (seq_t)1;

    unsigned len = bit_length(m);
    bool second_bit = (((m >> (len - 2)) & 1) != 0);

    if (!second_bit) {
        /* starts with "10": append a 1 bit */
        return (m << 1) | 1;
    }
    if ((m & 1) == 0) {
        /* starts with "11", even: delete last 0 bit */
        return m >> 1;
    }
    /* starts with "11", odd: "10" + complement of the remaining bits */
    unsigned k = len - 2;
    seq_t mask = ((seq_t)1 << k) - 1;
    seq_t s = m & mask;
    seq_t comp_s = mask ^ s;
    return ((seq_t)2 << k) | comp_s;
}
