/*
 * A079243  Number of isomorphism classes of associative non-commutative
 *          non-anti-associative anti-commutative closed binary operations on
 *          a set of order n.
 * https://oeis.org/A079243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of positive divisors of n for n>1, a(0)=0, a(1)=0
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    idx_t m = n;
    seq_t divs = 1;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            divs *= (seq_t)(exp + 1);
        }
    }
    if (m > 1)           /* remaining prime factor */
        divs *= (seq_t)2;

    return divs;
}
