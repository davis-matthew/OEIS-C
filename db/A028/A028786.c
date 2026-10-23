/*
 * A028786  Nonsquares mod 73.
 * https://oeis.org/A028786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nonsquare[(n-1)%36] + 73 * ⌊(n-1)/36⌋ where nonsquare[]
 *            is the sorted list of quadratic non‑residues modulo 73
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t nonsq[36] = {
        5,  7, 10, 11, 13, 14, 15, 17,
        20, 21, 22, 26, 28, 29, 30, 31,
        33, 34, 39, 40, 42, 43, 44, 45,
        47, 51, 52, 53, 56, 58, 59, 60,
        62, 63, 66, 68
    };
    idx_t k = n - 1;                    // convert to zero‑based index
    idx_t block = k / 36;               // how many complete 73‑length blocks
    idx_t offset = k % 36;              // position in the block
    return (seq_t)nonsq[offset] + (seq_t)73 * (seq_t)block;
}
