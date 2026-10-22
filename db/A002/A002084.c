/*
 * A002084  Sinh(x) / cos(x) = Sum_{n>=0} a(n)*x^(2n+1)/(2n+1)!.
 * https://oeis.org/A002084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   from sinh(x) = cos(x) * Sum a(n) x^(2n+1)/(2n+1)!: a(n) = 1 +
 *            Sum_{k=1..n} (-1)^(k+1) * binomial(2n+1, 2k) * a(n-k), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/a001028_z.h"
#include "../utils/a001028_grow.h"

static seq_t *A2084 = NULL;   /* computed terms */
static seq_t *R2084 = NULL;   /* Pascal row 2*cnt-1 */
static idx_t cnt2084 = 0;     /* number of terms computed */
static idx_t capA2084 = 0;
static idx_t capR2084 = 0;

seq_t a(idx_t n) {
    if (cnt2084 == 0) {
        capA2084 = 16;
        capR2084 = 2 * capA2084 + 2;
        A2084 = a001028_z(capA2084);
        R2084 = a001028_z(capR2084);
        A2084[0] = 1;
        R2084[0] = 1;
        R2084[1] = 1;          /* row 1 */
        cnt2084 = 1;
    }
    if (n < cnt2084) return A2084[n];

    if (capA2084 < n + 1) {
        idx_t nc = capA2084;
        while (nc < n + 1) nc *= 2;
        A2084 = a001028_grow(A2084, capA2084, nc);
        capA2084 = nc;
        idx_t nr = 2 * nc + 2;
        R2084 = a001028_grow(R2084, capR2084, nr);
        capR2084 = nr;
    }

    for (idx_t m = cnt2084; m <= n; m++) {
        /* advance Pascal row from 2m-1 to 2m+1 */
        for (idx_t step = 0; step < 2; step++) {
            idx_t r = 2 * m - 1 + step;   /* current row index */
            R2084[r + 1] = 1;
            for (idx_t j = r; j >= 1; j--) {
                R2084[j] += R2084[j - 1];
            }
        }
        seq_t pos = 1, neg = 0;
        for (idx_t k = 1; k <= m; k++) {
            seq_t t = R2084[2 * k] * A2084[m - k];
            if (k & 1) pos += t;
            else neg += t;
        }
        A2084[m] = pos - neg;
        cnt2084 = m + 1;
    }
    return A2084[n];
}
