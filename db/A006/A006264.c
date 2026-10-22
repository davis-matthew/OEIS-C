/*
 * A006264  Diagonal length function.
 * https://oeis.org/A006264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(floor(log_2(n))) for n >= 1, a(0) = 0, where f(k) = 0,
 *            1, 4, 5, 8, 9, 10 for k = 0..6 (the known values of the diagonal
 *            length function at powers of 2); past k = 6 the code assumes
 *            f(k) = f(k-1) + 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int A006264_f[7] = {0, 1, 4, 5, 8, 9, 10};

seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t k = 0;
    idx_t t = n;
    while (t > 1) {
        t >>= 1;
        k++;
    }

    if (k <= 6) return (seq_t)A006264_f[k];
    return (seq_t)A006264_f[6] + (seq_t)(k - 6);
}
