/*
 * A001194  a(n) = A059366(n,n-2) = A059366(n,2) for n >= 2, where the
 *          triangle A059366 arises in the expansion of a trigonometric
 *          integral.
 * https://oeis.org/A001194
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n*(n-1)/2 * (2*n-5)!! (equivalently
 *            3*n*(n-1)*(2*n-4)!/(2^(n-1)*(n-2)!)), n >= 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t h = (n % 2 == 0) ? (n / 2) * (n - 1) : ((n - 1) / 2) * n;
    seq_t r = (seq_t)3 * (seq_t)h;
    idx_t k;
    for (k = 3; k + 5 <= 2 * n; k += 2) {
        r *= (seq_t)k;
    }
    return r;
}
