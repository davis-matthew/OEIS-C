/*
 * A003091  a(n) = floor( 2^(n*(n-1)/2) / n! ).
 * https://oeis.org/A003091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( 2^binomial(n,2) / n! )
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   a003091_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003091_is_big.h"

seq_t a(idx_t n) {
    idx_t E = n * (n - 1) / 2;
    if (a003091_is_big()) {
        seq_t p = 1;
        seq_t f = 1;
        idx_t k;
        p <<= E;
        for (k = 2; k <= n; k++) f *= (seq_t)k;
        return p / f;
    } else {
        /* Fixed width: stream the bits of 2^E through a chain of
           long divisions by 2, 3, ..., n, so that no intermediate
           exceeds the final quotient (floor(floor(x/a)/b) = floor(x/(ab))). */
        uint64_t r[32];
        seq_t q = 0;
        idx_t i, k;
        if (n > 30) return 0; /* value cannot fit in 128 bits */
        for (k = 0; k < 32; k++) r[k] = 0;
        for (i = 0; i <= E; i++) {
            uint64_t b = (i == 0) ? 1 : 0;
            for (k = 2; k <= n; k++) {
                r[k] = 2 * r[k] + b;
                if (r[k] >= k) { r[k] -= k; b = 1; } else b = 0;
            }
            q = q * 2 + (seq_t)b;
        }
        return q;
    }
}
