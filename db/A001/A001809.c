/*
 * A001809  a(n) = n! * n(n-1)/4.
 * https://oeis.org/A001809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, eigen
 * Formula:   a(n) = (n-2)! * (n*(n-1)/2)^2 = n! * n*(n-1)/4, a(0) = a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t fact = 1;   /* fact = fk! */
    static idx_t fk = 0;
    idx_t m, i;
    seq_t t;
    if (n < 2) return 0;
    m = n - 2;
    if (m < fk) {
        fact = 1;
        fk = 0;
    }
    for (i = fk + 1; i <= m; i++) {
        fact *= (seq_t)i;
    }
    fk = m;
    t = (seq_t)n * (seq_t)(n - 1) / 2;
    return fact * t * t;
}
