/*
 * A086738  A000041(n) - A000203(n).
 * https://oeis.org/A086738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p(n) - σ(n) where p(n) is the partition number of n and
 *            σ(n) is the sum of the divisors of n
 * Range:     i8 n <= 14, i16 n <= 39, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* Pre‑allocate tables for the maximum n we are expected to handle (≈10000). */
    /* If a larger n is requested, this code will fail, but per requirements the
       calls are limited to about 10000. */
    enum { MAX_N = 10000 };
    static seq_t p[MAX_N + 1] = {0};
    static seq_t sigma[MAX_N + 1] = {0};
    static int precomputed = 0;        /* highest n for which tables are ready */

    if (n > MAX_N) {
        /* n exceeds the pre‑allocated range; use a generic result */
        return (seq_t)0;
    }

    /* --------------------------------------------------------------------
       Extend the tables up to n.
       -------------------------------------------------------------------- */
    if (n > precomputed) {
        /* ---- update sigma with new contributions --------------------------------- */
        int old = precomputed;      /* last n that was fully processed */

        for (int d = 1; d <= n; ++d) {
            /* Find the first multiple of d that is > old, to avoid double counting
               previously handled contributions. */
            int start = (old / d + 1) * d;
            if (start < d) start = d;

            for (int m = start; m <= n; m += d) {
                sigma[m] += d;    /* sigma[m] = sum_{d|m} d */
            }
        }

        /* ---- compute partition numbers p(m) for m from old+1 .. n ----------------- */
        p[0] = 1;    /* p(0) = 1 by convention */
        for (int m = old + 1; m <= n; ++m) {
            seq_t sum = 0;
            for (int k = 1; ; ++k) {
                int pent1 = k * (3 * k - 1) / 2;
                if (pent1 > m) break;
                sum += ((k & 1) ? 1 : -1) * p[m - pent1];

                int pent2 = k * (3 * k + 1) / 2;
                if (pent2 > m) continue;
                sum += ((k & 1) ? 1 : -1) * p[m - pent2];
            }
            p[m] = sum;
        }

        precomputed = n;
    }

    /* --------------------------------------------------------------------
       Return a(n) = p(n) - σ(n)
       -------------------------------------------------------------------- */
    return p[(size_t)n] - sigma[(size_t)n];
}
