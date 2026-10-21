/*
 * A058764  Smallest number x such that cototient(x) = 2^n.
 * https://oeis.org/A058764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(0)=2, a(1)=4, a(n)=3*2^(n-1) for n>1 (conjectured from OEIS
 *            A058764)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)4;

    /* Compute 2^(n-1) by repeated doubling, then multiply by 3 */
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        val <<= 1;          /* val = val * 2 */

    val *= (seq_t)3;
    return val;
}
