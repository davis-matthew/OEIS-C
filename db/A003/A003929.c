/*
 * A003929  Order of simple Chevalley group B_n (7).
 * https://oeis.org/A003929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^(n^2) * Product_{i=1..n} (7^(2*i) - 1) / 2 (order of
 *            B_n(7) = b(7,n) of A003927)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a003929_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003929_ipow.h"

seq_t a(idx_t n) {
    /* (7^2 - 1)/2 = 24: divide by 2 up front so everything else is exact multiplication */
    seq_t result = 24;
    seq_t t = 49;
    for (idx_t i = 2; i <= n; i++) {
        t *= 49;
        result *= (t - 1);
    }
    seq_t p = a003929_ipow((seq_t)7, n);
    result *= a003929_ipow(p, n);
    return result;
}
