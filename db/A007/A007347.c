/*
 * A007347  Maximal Eulerian numbers of second kind.
 * https://oeis.org/A007347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k=0..n} A201637(n, k), where A201637 is the
 *            second-order Eulerian triangle T(n,k) = (k+1)*T(n-1,k) +
 *            (2n-k-1)*T(n-1,k-1), T(0,0) = 1, T(n,k) = 0 for k > n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n)
{
    seq_t *t;
    seq_t best;
    idx_t row, k;

    if (n == 0) {
        return 1;
    }

    t = alloc_zero(n + 1);
    t[0] = 1;

    for (row = 1; row <= n; row++) {
        /* update in place from high k down so t[k-1] is still the old row */
        for (k = row; k >= 1; k--) {
            t[k] = (seq_t)(k + 1) * t[k] + (seq_t)(2 * row - k - 1) * t[k - 1];
        }
        /* t[0] stays 1 */
    }

    best = t[0];
    for (k = 1; k <= n; k++) {
        if (t[k] > best) {
            best = t[k];
        }
    }

    release(t, n + 1);
    return best;
}
