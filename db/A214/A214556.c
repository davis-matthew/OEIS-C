/*
 * A214556  Fixed points of the Kaprekar map A151949, of the form
 *          63{n}176{n}4, where d{n} is digit d repeated n times, n >= 0.
 * https://oeis.org/A214556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (190*100^(n+1) - 47*10^(n+1) - 8)/3
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 7, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 100^(n+1) = 10^(2*(n+1)) */
    seq_t pow2n1 = pow10(2 * (n + 1));
    seq_t pown1  = pow10(n + 1);
    seq_t tmp    = ((seq_t)190) * pow2n1 - ((seq_t)47) * pown1 - ((seq_t)8);
    return tmp / 3;
}
