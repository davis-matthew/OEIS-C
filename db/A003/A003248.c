/*
 * A003248  a(n) = A000201(A003234(n)) + n.
 * https://oeis.org/A003248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000201(A003234(n)) + n, where A003234 is found by
 *            search: the n-th m in A000201 (m = floor(k*phi)) with A003231(m)
 *            = 2*m + floor(m*phi) in A001950. floor(y*phi) = (y +
 *            isqrt(5*y^2)) / 2, computed exactly in integers
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   s3248_isqrt, s3248_lw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/s3248_isqrt.h"
#include "../utils/s3248_lw.h"

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t m;
    for (m = 1;; m++) {
        uint64_t lm = s3248_lw(m);
        if (s3248_lw(m + 1) - lm != 2) continue;      /* m must be in A000201 */
        {
            uint64_t x = 2 * m + lm;                   /* A003231(m) */
            if (s3248_lw(x + 1) - s3248_lw(x) != 1) continue; /* x in A001950 */
        }
        if (++count == (uint64_t)n)
            return (seq_t)lm + (seq_t)n;
    }
}
