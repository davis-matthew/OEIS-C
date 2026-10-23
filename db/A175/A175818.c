/*
 * A175818  Concatenate all previous terms in base 2 and add 1.
 * https://oeis.org/A175818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 1 + concat of binary representations of a(1) ... a(n-1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t pref = (seq_t)1;              /* concatenation of a(1)..a(k) so far */
    for (idx_t k = 2; k < n; ++k) {
        seq_t ak   = pref + (seq_t)1;   /* a(k) = pref + 1 */
        unsigned len = bitlen_seq_t(ak); /* number of bits in a(k) */
        seq_t shift = pow2((idx_t)len);  /* 2^len */
        pref = pref * shift + ak;        /* update concatenation */
    }
    return pref + (seq_t)1;             /* a(n) = pref + 1 */
}
