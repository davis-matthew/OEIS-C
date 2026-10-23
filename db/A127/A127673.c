/*
 * A127673  One half of even powers of 2*x in terms of Chebyshev's
 *          T-polynomials.
 * https://oeis.org/A127673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(2*r, k) where r is the row index such that *
 *            r(r+1)/2 <= n < (r+1)(r+2)/2, k = n - r(r+1)/2; if k = r and r >
 *            0 * divide the binomial by 2. Special case: a(0) = 1 (not 1/2)
 * Range:     u8 n <= 13, u16 n <= 42, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (true) {
        __int128 t = (__int128)(r + 1) * (r + 2);
        t >>= 1;                /* t = (r+1)*(r+2)/2 */
        if (t > n) break;
        ++r;
    }

    /* Compute the offset of the start of row r */
    __int128 offset128 = (__int128)r * (r + 1);
    offset128 >>= 1;            /* offset = r*(r+1)/2 */
    idx_t offset = (idx_t)offset128;

    idx_t k = n - offset;       /* position in row r */

    seq_t val;
    if (k == r) {
        val = binomial(2 * r, k);
        /* For r == 0, the intended value is 1 (not 1/2) */
        if (r == 0)
            val = 1;
        else
            val = val / 2;      /* divide by 2 for central coefficient */
    } else {
        val = binomial(2 * r, k);
    }

    return val;
}
