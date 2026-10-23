/*
 * A162215  a(n) is the smallest multiple of n that is greater than 2n and
 *          contains the same number of 1's in its binary representation as n
 *          contains.
 * https://oeis.org/A162215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A162215:  a(n) is the smallest multiple of n that is greater
   than 2n and contains the same number of 1's in its binary
   representation as n contains.
   For all n ≥ 1 we have a(n) = 3n if popcount(3n) = popcount(n),
   otherwise a(n) = 4n.
*/

#include <stdint.h>

static int popcount_seq_t(seq_t x)
{
    int cnt = 0;
    while (x) {
        cnt += (int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n=0, but safe */

    seq_t sn   = (seq_t)n;
    seq_t m3   = sn * (seq_t)3;
    seq_t m4   = sn * (seq_t)4;
    int pc_n   = popcount_seq_t(sn);
    int pc_m3  = popcount_seq_t(m3);

    return (pc_m3 == pc_n) ? m3 : m4;
}
