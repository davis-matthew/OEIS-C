/*
 * A014495  Central binomial coefficient - 1.
 * https://oeis.org/A014495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n, floor(n/2)) - 1 where C(n,k) is the binomial
 *            coefficient
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 130,
 *            big any n
 * Helpers:   _binom_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* util: binomial may be used for arbitrary-precision seq_t */
#include "../utils/binomial.h"
#include "../utils/_binom_u64.h"

seq_t a(idx_t n)
{
    /* For seq_t types that are 64 bits or less use the 128‑bit helper
       to avoid intermediate overflow. For larger types fall back to the
       arbitrary‑precision binomial routine provided by the build. */
    if (sizeof(seq_t) <= 8) {
        idx_t k = n >> 1;
        seq_t val = _binom_u64(n, k) - (seq_t)1;
        return val;
    } else {
        idx_t k = n >> 1;
        seq_t val = binomial(n, k) - (seq_t)1;
        return val;
    }
}
