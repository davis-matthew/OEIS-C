/*
 * A182027  a(n) = number of n-lettered words in the alphabet {1, 2} with as
 *          many occurrences of the substring (consecutive subword) [1, 1] as
 *          of [2, 2].
 * https://oeis.org/A182027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n=0), 2 (n=1); else if n is odd: C(n-1,(n-1)/2); if n
 *            is even: 2*C(n-2,(n-2)/2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n & 1) {           /* n odd */
        return binom(n - 1, (n - 1) / 2);
    } else {               /* n even */
        seq_t v = binom(n - 2, (n - 2) / 2);
        return v + v;      /* multiply by 2 */
    }
}
