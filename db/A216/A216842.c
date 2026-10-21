/*
 * A216842  Degree of the smallest irreducible polynomial factor for the
 *          polynomial (x^(2n+1)+1)/(x+1) over GF(2).
 * https://oeis.org/A216842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ ord_2(d) : d | (2n+1), d > 1 }, with a(0)=0
 *            (ord_2(d) = multiplicative order of 2 modulo d) For n=0 the
 *            polynomial is 1, so the answer is 0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   order_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/order_of.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t m = 2 * n + 1;          /* m is odd and >= 3 for n > 0 */
    seq_t min_order = 0;
    bool found = false;

    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;
            if (d1 > 1) {
                seq_t ord = order_of(d1);
                if (!found || ord < min_order) {
                    min_order = ord;
                    found = true;
                }
            }

            idx_t d2 = m / i;
            if (d2 != d1 && d2 > 1) {
                seq_t ord = order_of(d2);
                if (!found || ord < min_order) {
                    min_order = ord;
                    found = true;
                }
            }
        }
    }

    return min_order;
}
