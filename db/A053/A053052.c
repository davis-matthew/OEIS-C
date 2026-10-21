/*
 * A053052  Append n to the previous term, reverse alternate terms.
 * https://oeis.org/A053052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = if n is odd then concatenate 1,2,…,n else concatenate
 *            n,n-1,…,1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    if ((n & (idx_t)1) == (idx_t)1) {               /* odd n: ascending */
        for (idx_t i = 1; i <= n; ++i) {
            idx_t d = (idx_t)digit_count((uint64_t)i);
            res = res * pow10(d) + (seq_t)i;
        }
    } else {                                       /* even n: descending */
        for (idx_t i = n; i > 0; --i) {
            idx_t d = (idx_t)digit_count((uint64_t)i);
            res = res * pow10(d) + (seq_t)i;
        }
    }
    return res;
}
