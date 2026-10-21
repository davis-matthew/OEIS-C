/*
 * A084183  Jacobsthal reverse-pair sequence.
 * https://oeis.org/A084183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(n)=(2^(n+1)-1)/3 if n is odd, a(n)=(2^(n-1)+1)/3 if n
 *            is even and n>0 (which is equivalent to the OEIS formula
 *            a(n)=(5*2^n-6*0^n+4(-1)^n-3(-2)^n)/12)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (n & 1ULL) {            /* n is odd */
        seq_t num = pow2(n + 1) - (seq_t)1;
        return num / (seq_t)3;
    } else {                   /* n is even (n>0) */
        seq_t num = pow2(n - 1) + (seq_t)1;
        return num / (seq_t)3;
    }
}
