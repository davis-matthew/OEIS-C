/*
 * A003926  Order of universal Chevalley group B_n (9).
 * https://oeis.org/A003926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(n^2) * Product_{i=1..n} (9^(2*i) - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   a003926_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003926_powmod.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t p = 1;
    for (idx_t i = 1; i <= n; i++) {
        p = p * 81;
        result = result * (p - 1);
    }
    result = result * a003926_powmod((seq_t)9, n * n);
    return result;
}
