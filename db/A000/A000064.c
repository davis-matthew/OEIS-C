/*
 * A000064  Partial sums of (unordered) ways of making change for n cents
 *          using coins of 1, 2, 5, 10 cents.
 * https://oeis.org/A000064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{t>=0, f>=0, 10t+5f<=n} g(n-10t-5f), where g(m) =
 *            (floor(m/2)+1)*(m+1-floor(m/2)) counts the partitions of m into
 *            two kinds of 1 and parts 2 (G.f.
 *            1/((1-x)^2(1-x^2)(1-x^5)(1-x^10)))
 * Range:     u8 n <= 19, u16 n <= 102, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t t10, r, m, q;
    for (t10 = 0; t10 <= n; t10 += 10) {
        r = n - t10;
        for (m = r; ; m -= 5) {
            q = m / 2;
            total += ((seq_t)q + 1) * ((seq_t)m + 1 - (seq_t)q);
            if (m < 5) break;
        }
    }
    return total;
}
