/*
 * A003320  a(n) = max_{k=0..n} k^(n-k).
 * https://oeis.org/A003320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = max_{k=0..n} k^(n-k). (n-k)*log(k) is concave in k, so a
 *            floating-point scan over the index finds the best k, and the few
 *            k next to it are then compared exactly in seq_t
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   a3320_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/a3320_pw.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t best = 1, k, lo, hi;
    double bestf = -1.0;

    if (n < 2) return result;

    for (k = 1; k <= n; k++) {
        double f = (double)(n - k) * log((double)k);
        if (f > bestf) {
            bestf = f;
            best = k;
        }
    }

    lo = (best > 3) ? best - 2 : 1;
    hi = (best + 2 <= n) ? best + 2 : n;
    for (k = lo; k <= hi; k++) {
        seq_t v = a3320_pw((seq_t)k, n - k);
        if (v > result) result = v;
    }
    return result;
}
