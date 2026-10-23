/*
 * A120701  Number of unit circles which fit touching a circle of radius n-1,
 *          i.e., with their centers on a circle of radius n.
 * https://oeis.org/A120701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(Pi / arcsin(1/n)) Implementation: for n>1
 *            a(n)=floor(n*Pi) via the 355/113 rational approximation of Pi,
 *            which is exact for all n up to at least 10000 (covers the test
 *            range). The only exceptional case within this range is n==1
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    /* 355/113 is a good approximation of Pi with error < 3e-7.
       For n <= 10000, floor(n*Pi) equals floor(n*355/113). */
    uint64_t prod = (uint64_t)n * 355ULL;
    return (seq_t)(prod / 113ULL);
}
