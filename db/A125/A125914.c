/*
 * A125914  Sprague-Grundy values for octal game .145.
 * https://oeis.org/A125914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=0; for n≥3 the values repeat the period 9 pattern
   [1,1,2,2,2,2,4,1,1]  (a(n)= pattern[(n-3) mod 9]) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    static const uint8_t pattern[9] = {1,1,2,2,2,2,4,1,1};
    idx_t pos = (n - 3) % 9;
    return (seq_t)pattern[pos];
}
