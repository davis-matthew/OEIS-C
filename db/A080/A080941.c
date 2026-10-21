/*
 * A080941  Greatest proper divisor of n which is a suffix of n in binary
 *          representation; a(n) = 0 if no such divisor exists.
 * https://oeis.org/A080941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if n is a power of two; otherwise a(n) = max{ d | d
 *            divides n, d < n, n mod 2^k == d, where k = bitlen(d) }
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* n is a power of two ⇒ no proper divisor is a suffix */
    if ((n & (n - 1)) == 0) return (seq_t)0;

    seq_t best = (seq_t)0;
    idx_t nn   = n;  /* local copy for the divisor loop */

    for (idx_t i = 1; i <= nn / i; ++i) {
        if (nn % i == 0) {
            idx_t d1 = i;
            idx_t d2 = nn / i;

            /* Check divisor d1 */
            if (d1 != nn) {
                unsigned len  = bitlen_seq_t((seq_t)d1);
                seq_t mask    = (((seq_t)1) << len) - (seq_t)1;
                if (((seq_t)nn & mask) == (seq_t)d1) {
                    if ((seq_t)d1 > best) best = (seq_t)d1;
                }
            }

            /* Check divisor d2 (different from d1 and nn) */
            if (d2 != nn && d2 != d1) {
                unsigned len  = bitlen_seq_t((seq_t)d2);
                seq_t mask    = (((seq_t)1) << len) - (seq_t)1;
                if (((seq_t)nn & mask) == (seq_t)d2) {
                    if ((seq_t)d2 > best) best = (seq_t)d2;
                }
            }
        }
    }
    return best;
}
