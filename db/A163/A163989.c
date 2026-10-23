/*
 * A163989  First differences of A160379.
 * https://oeis.org/A163989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(2) = 26 a(2n) = 132n^2 - 240n + 152 for n >= 2
 *            a(2n+1) = 6n^2 + 2 for n >= 1 (also satisfies a(n) =
 *            3*a(n-2)-3*a(n-4)+a(n-6) for n > 8)
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)26;
    }
    if ((n & 1) == 0) {           /* even index: n = 2k,  k >= 2 */
        idx_t k = n / 2;
        seq_t kk = (seq_t)k;
        /* a = 132*k^2 - 240*k + 152 */
        return (seq_t)132 * kk * kk - (seq_t)240 * kk + (seq_t)152;
    } else {                       /* odd index: n = 2k+1,  k >= 1 */
        idx_t k = (n - 1) / 2;
        seq_t kk = (seq_t)k;
        /* a = 6*k^2 + 2 */
        return (seq_t)6 * kk * kk + (seq_t)2;
    }
}
