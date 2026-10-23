/*
 * A026395  a(n) = 5*a(n-2), starting 1,2,4.
 * https://oeis.org/A026395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) for n>=3, with a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    if ((n & 1) == 0) {          /* n is even */
        idx_t k = n / 2;          /* k >= 1 for n >= 2 */
        seq_t p = big_pow((seq_t)5, (seq_t)(k - 1));
        return p * (seq_t)4;
    } else {                     /* n is odd */
        idx_t k = (n - 1) / 2;    /* k >= 0 */
        seq_t p = big_pow((seq_t)5, (seq_t)k);
        return p * (seq_t)2;
    }
}
