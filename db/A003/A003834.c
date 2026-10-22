/*
 * A003834  Order of universal Chevalley group D_n (7).
 * https://oeis.org/A003834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n*(n-1)) * (7^n - 1) * Product_{i=1..n-1} (7^(2*i) -
 *            1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t result = a000248_pow((seq_t)7, n * (n - 1));
    seq_t qn = a000248_pow((seq_t)7, n);
    result *= (qn - 1);
    seq_t q2i = 1;
    for (idx_t i = 1; i < n; i++) {
        q2i *= 49;
        result *= (q2i - 1);
    }
    return result;
}
