/*
 * A003440  Number of binary vectors with restricted repetitions.
 * https://oeis.org/A003440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (C(k, n-k) + C(k+1, n-k-1))^2/2 for n>0,
 *            with a(0)=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   a3440_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a3440_binom.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* Accumulate floor(t^2/2) per term plus a count of odd t, so the
       running sum never exceeds a(n) (avoids overflowing 2*a(n)). */
    seq_t sum = 0;
    idx_t odd = 0;
    for (idx_t k = 0; k <= n; k++) {
        seq_t t = a3440_binom(k, n - k);
        if (k < n) t += a3440_binom(k + 1, n - k - 1);
        if (t == 0) continue;
        if ((t & 1) == 0) {
            sum += (t / 2) * t;
        } else {
            sum += ((t - 1) / 2) * (t + 1);
            odd++;
        }
    }
    sum += (seq_t)(odd / 2);
    return sum;
}
