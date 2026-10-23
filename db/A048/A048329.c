/*
 * A048329  Numbers that are repdigits in base 4.
 * https://oeis.org/A048329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0; for n>0, a(n)=((4^((n-1)/3+1)-1)/3)*(((n-1)%3)+1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 96, u128 n <= 192,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* compute k and digit d for the n-th repdigit in base 4 */
    idx_t k = (n - 1) / 3 + 1;
    idx_t d = (n - 1) % 3 + 1;

    seq_t base = pow4(k);          /* 4^k */
    seq_t val = base - 1;          /* 4^k - 1 */
    val = val / 3;                  /* (4^k - 1)/3  */
    val = val * (seq_t)d;           /* multiply by digit d (1,2,3) */
    return val;
}
