/*
 * A002884  Number of nonsingular n X n matrices over GF(2) (order of the
 *          group GL(n,2)); order of Chevalley group A_n (2); order of
 *          projective special linear group PSL_n(2).
 * https://oeis.org/A002884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n*(n-1)/2) * Product_{i=1..n} (2^i - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   a002884_is_big, a002884_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002884_is_big.h"
#include "../utils/a002884_prod.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (!a002884_is_big()) {
        /* fixed width: a(n) = Product_{i=1..n} 2^(i-1)*(2^i-1), modular */
        seq_t r = 1, p = 1;
        idx_t i;
        for (i = 1; i <= n; i++) {
            seq_t q = p + p;
            r = r * p * (q - 1);
            if (r == 0) break;
            p = q;
        }
        return r;
    }
    seq_t r = a002884_prod(1, n);
    idx_t sh = n * (n - 1) / 2;
    r <<= sh;
    return r;
}
