/*
 * A164532  a(n) = 6*a(n-2) for n > 2; a(1) = 1, a(2) = 4.
 * https://oeis.org/A164532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2); a(1)=1, a(2)=4
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 4;

    if (n & 1) {                           /* odd n */
        idx_t exp = (n - 1) >> 1;          /* (n-1)/2 */
        return big_pow((seq_t)6, (seq_t)exp);
    } else {                               /* even n */
        idx_t exp = (n >> 1) - 1;          /* n/2 - 1 */
        seq_t pow6 = big_pow((seq_t)6, (seq_t)exp);
        return pow6 * ((seq_t)4);
    }
}
