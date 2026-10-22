/*
 * A003130  Impedances of an n-terminal network.
 * https://oeis.org/A003130
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} Stirling2(n,k) * w(k), where w(k) = ((k)_2/2
 *            + (k)_3 + 11*(k)_4/4 + 5*(k)_5/4 + (k)_6/8) = C(k,2) * (1 + 2m +
 *            11*C(m,2) + 15*C(m,3) + 6*C(m,4)), m = k-2 (this is A003128(n) +
 *            2*A003129(n) + U(n) with U(n) = Sum_k u(k)*Stirling2(n,k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   A_grow, weight
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A_grow.h"
#include "../utils/weight.h"

static seq_t *row_buf = 0;
static idx_t row_cap = 0;

seq_t a(idx_t n)
{
    idx_t i, k;
    seq_t s = 0;
    if (n < 2) return 0;
    if (row_cap < n + 1) {
        row_buf = A_grow(row_buf, row_cap, n + 1);
        row_cap = n + 1;
    }
    for (k = 0; k <= n; k++) row_buf[k] = 0;
    row_buf[0] = 1;
    for (i = 1; i <= n; i++) {
        for (k = i; k >= 1; k--) {
            row_buf[k] = row_buf[k] * (seq_t)k + row_buf[k - 1];
        }
        row_buf[0] = 0;
    }
    for (k = 2; k <= n; k++) {
        s += weight(k) * row_buf[k];
    }
    return s;
}
