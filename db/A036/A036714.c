/*
 * A036714  a(n)=number of Gaussian integers z=a+bi satisfying
 *          n-1/2<|z|<=n+1/2, a>=0, 0<=b<a.
 * https://oeis.org/A036714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Gaussian integers * a+bi with a>=1, 0<=b<a such
 *            that (n-1/2)^2 < a^2+b^2 <= (n+1/2)^2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    /* integer boundaries of the annulus, multiplied by 4 to avoid fractions   */
    const idx_t low  = 4*n*n - 4*n + 1;   /* 4*(n-1/2)^2   */
    const idx_t high = 4*n*n + 4*n + 1;   /* 4*(n+1/2)^2   */

    /* a ranges from 1 up to n+1 (sufficient for all points with |z|<=n+½) */
    for (idx_t a = 1; a <= n + 1; ++a) {
        const idx_t a2 = a*a;
        for (idx_t b = 0; b < a; ++b) {
            const idx_t s4 = 4 * (a2 + b*b);   /* 4*(a^2+b^2)   */
            if (s4 > low && s4 <= high) {
                ++count;
            }
        }
    }
    return count;
}
