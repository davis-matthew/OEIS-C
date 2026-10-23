/*
 * A117868  Number of palindromes (in base 9) below 9^n.
 * https://oeis.org/A117868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*9^((n-1)/2)-2 if n is odd, a(n) = 2*9^(n/2)-2 if n is
 *            even Initial values: a(1)=8, a(2)=16 (and so on)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1, safe guard */
    if (n & 1) {                       /* n odd */
        int exp = (int)((n - 1) / 2);
        seq_t p = pow_seq((seq_t)9, exp);
        return (seq_t)10 * p - (seq_t)2;
    } else {                           /* n even */
        int exp = (int)(n / 2);
        seq_t p = pow_seq((seq_t)9, exp);
        return (seq_t)2 * p - (seq_t)2;
    }
}
