/*
 * A145818  Odd positive integers a(n) such that for every integer m == 3 (mod
 *          4) there exists a unique representation of the form m = a(l) +
 *          2*a(s), but there are no such representations for m == 1 (mod 4).
 * https://oeis.org/A145818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1 + 4 * Σ_{i≥0} bit_i(n-1) * 4^i
 * i.e. take n-1, interpret each binary bit as a digit in base‑4 (0 or 1),
 * then multiply the resulting base‑4 number by 4 and add 1.
 * This yields all odd integers whose binary expansion has 0 in every odd
 * position (counting LSB as position 0).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by contract */
    idx_t m = n - 1;        /* bits that will be placed on even positions */
    seq_t result = 1;       /* the mandatory bit at position 0 (value 1) */
    seq_t shift  = 4;       /* corresponds to 2^(2) = 4, first even position >0 */

    while (m != 0) {
        if (m & 1) {
            result += shift;      /* set the bit at the appropriate even position */
        }
        shift <<= 2;              /* move to the next even position (×4) */
        m >>= 1;
    }

    return result;
}
