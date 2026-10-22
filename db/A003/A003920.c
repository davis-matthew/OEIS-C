/*
 * A003920  Order of universal Chevalley group B_n (3).
 * https://oeis.org/A003920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n^2) * Product_{k=1..n} (3^(2*k) - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   a003920_powmod_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003920_powmod_free.h"

seq_t a(idx_t n) {
    seq_t result = a003920_powmod_free((seq_t)3, n * n);
    seq_t power = (seq_t)9;
    idx_t k;
    for (k = 1; k <= n; k++) {
        result = result * (power - 1);
        power = power * 9;
    }
    return result;
}
