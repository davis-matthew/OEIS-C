/*
 * A178753  a(n) = n XOR floor(Log_2(n))+1, where XOR is bitwise XOR.
 * https://oeis.org/A178753
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n XOR (floor(Log_2(n))+1)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n<2, safe fallback */
    unsigned long long v = (unsigned long long)n;
    /* floor_log2(n) = 63 - clz(v).  clz(v) is number of leading zeros. */
    unsigned int shift = 64 - __builtin_clzll(v);  /* floor_log2(n) + 1 */
    return (seq_t)v ^ (seq_t)shift;
}
