/*
 * A097791  a(n)=5a(n-1)+C(n+4,4),n>0, a(0)=1.
 * https://oeis.org/A097791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1)+C(n+4,4), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {/* compute a(i) from a(i-1) */
        res = res * (seq_t)5 + binomial(i + (idx_t)4, (idx_t)4);
    }
    return res;
}
