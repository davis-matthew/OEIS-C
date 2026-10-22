/*
 * A004212  Shifts one place left under 3rd-order binomial transform.
 * https://oeis.org/A004212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k)*Stirling2(n,k), computed via T(n,k)
 *            = 3^(n-k)*Stirling2(n,k), T(n,k) = 3*k*T(n-1,k) + T(n-1,k-1),
 *            T(0,0) = 1, T(n,0) = 0 for n > 0; a(n) = Sum_k T(n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *row = 0;
static seq_t *vals = 0;
static idx_t done = 0;

seq_t a(idx_t n) {
    if (n < done) return vals[n];

    row = grow_zero(row, done, n + 1);
    vals = grow_zero(vals, done, n + 1);

    idx_t m = done;
    if (done == 0) {
        row[0] = 1;
        vals[0] = 1;
        m = 1;
    }
    for (; m <= n; m++) {
        for (idx_t k = m; k >= 1; k--) {
            row[k] = (seq_t)(3 * k) * row[k] + row[k - 1];
        }
        row[0] = 0;
        seq_t s = 0;
        for (idx_t k = 0; k <= m; k++) s += row[k];
        vals[m] = s;
    }
    done = n + 1;
    return vals[n];
}
