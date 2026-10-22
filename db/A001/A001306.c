/*
 * A001306  Number of ways of making change for n cents using coins of 1, 5,
 *          10, 20, 50, 100 cents.
 * https://oeis.org/A001306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 1/((1-x)*(1-x^5)*(1-x^10)*(1-x^20)*(1-x^50)*(1-x^100)).
 *            Computed by summing over the counts of 100-, 50- and 20-cent
 *            coins. The remainder r is paid with 1, 5 and 10 cent coins. With
 *            m = floor(r/5) and q = floor(m/2), that can be done in
 *            (q+1)*(m+1) - q*(q+1) ways
 * Range:     u8 n <= 94, u16 n <= 434, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t h, f, t;
    for (h = 0; 100 * h <= n; h++) {
        idx_t r0 = n - 100 * h;
        for (f = 0; 50 * f <= r0; f++) {
            idx_t r1 = r0 - 50 * f;
            for (t = 0; 20 * t <= r1; t++) {
                uint64_t m = (uint64_t)((r1 - 20 * t) / 5);
                uint64_t q = m / 2;
                uint64_t w = (q + 1) * (m + 1) - q * (q + 1);
                s += (seq_t)w;
            }
        }
    }
    return s;
}
