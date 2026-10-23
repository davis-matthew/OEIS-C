/*
 * A011657  A binary m-sequence: expansion of reciprocal of x^3 + x + 1 (mod
 *          2, shifted by 2 initial 0's).
 * https://oeis.org/A011657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 7 ∈ {2,3,4,6}, otherwise 0 (binary m‑sequence
 *            of period 7)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[7] = {0,0,1,1,1,0,1};
    return pat[(size_t)(n % 7)];
}
