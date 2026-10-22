/*
 * A003921  Order of universal Chevalley group B_n (4).
 * https://oeis.org/A003921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n^2) * Product_{i=1..n} (16^i - 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   pw_A003921
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw_A003921.h"

seq_t a(idx_t n) {
    seq_t prod = 1;
    seq_t p = 1;
    for (idx_t i = 1; i <= n; i++) {
        p *= 16;
        prod *= p - 1;
    }
    return prod * pw_A003921((seq_t)4, n * n);
}
