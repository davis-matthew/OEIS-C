/*
 * A003837  Order of (usually) simple Chevalley group D_n (3).
 * https://oeis.org/A003837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n*(n-1)) * ((3^n - 1)/gcd(4, 3^n - 1)) *
 *            Product_{i=1..n-1} (3^(2*i) - 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t pow3n = a000272_ipow((seq_t)3, n);
    /* gcd(4, 3^n - 1) is 2 for odd n and 4 for even n */
    seq_t divisor = (n % 2 == 0) ? (seq_t)4 : (seq_t)2;
    seq_t result = (pow3n - 1) / divisor;

    seq_t p = (seq_t)1;
    for (idx_t i = 1; i < n; i++) {
        p *= 9;
        result *= (p - 1);
    }

    result *= a000272_ipow((seq_t)3, n * (n - 1));
    return result;
}
