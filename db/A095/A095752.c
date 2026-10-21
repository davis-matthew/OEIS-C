/*
 * A095752  a(n) = (1/9)*(-1+10^n)*(4*n+3).
 * https://oeis.org/A095752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/9)*(-1+10^n)*(4*n+3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_seq(idx_t e)
{
    seq_t res = 1;
    seq_t base = 10;
    while (e) {
        if (e & 1ul)
            res *= base;
        e >>= 1ul;
        base *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t p10   = pow10_seq(n);      /* 10^n   */
    seq_t term  = p10 - 1;           /* 10^n-1 */
    seq_t mult  = term * ((seq_t)n * 4 + 3);
    return mult / 9;                 /* divide by 9 */
}
