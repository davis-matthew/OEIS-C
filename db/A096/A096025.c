/*
 * A096025  Numbers n such that (n+j) mod (2+j) = 1 for j from 0 to 6 and
 *          (n+7) mod 9 <> 1.
 * https://oeis.org/A096025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A096025: numbers n such that n mod 840 = 3 and n mod 2520 ≠ 3.
    The sequence consists of the two arithmetic progressions
          843 + 2520·t   (t = 0,1,2,…)
          1683 + 2520·t  (t = 0,1,2,…)
    sorted together.  Hence
          a(2m–1) = 843 + 2520·(m–1)
          a(2m)   = 1683 + 2520·(m–1)
    for m ≥ 1. */

#include <stdint.h>   /* for uint64_t used by idx_t */

seq_t a(idx_t n)
{
    if (n == 0)                 /* not used (offset 1), but harmless */
        return (seq_t)0;

    /* k = floor((n-1)/2) for odd n, floor((n-2)/2) for even n */
    idx_t k = (n & 1) ? (n - 1) / 2 : (n - 2) / 2;

    /* base value depends on parity of n */
    seq_t base = (n & 1) ? (seq_t)843 : (seq_t)1683;

    /* return base + k * 2520 */
    return base + (seq_t)k * (seq_t)2520;
}
