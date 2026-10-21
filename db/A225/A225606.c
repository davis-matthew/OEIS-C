/*
 * A225606  Number of distinct lines passing through 3 or more points in an n
 *          X n grid.
 * https://oeis.org/A225606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n * + 2 * Σ_{1≤p,q≤⌊(n-1)/2⌋, gcd(p,q)=1} *
 *            [(n-2p)(n-2q) - max(0,(n-3p)(n-3q))]
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Euclidean GCD for 64‑bit unsigned integers. */

seq_t a(idx_t n) {
    if (n < 3) return 0;                /* no lines with 3+ points */

    seq_t N   = (seq_t)n;                /* use seq_t for all arithmetic */
    seq_t res = N + N;                  /* horizontal + vertical lines */

    idx_t max_idx = (n - 1) / 2;         /* floor((n-1)/2) */

    for (idx_t q = 1; q <= max_idx; ++q) {
        for (idx_t p = 1; p <= max_idx; ++p) {
            if (gcd_uint64(p, q) != 1) continue;  /* only reduced slopes */

            /* Lines with slope +p/q (and later -p/q). */
            seq_t t1 = (N - (seq_t)(2 * p)) * (N - (seq_t)(2 * q));

            seq_t t2 = 0;
            if (N > (seq_t)(3 * p) && N > (seq_t)(3 * q)) {
                t2 = (N - (seq_t)(3 * p)) * (N - (seq_t)(3 * q));
            }

            seq_t lines = t1 - t2;          /* lines with this slope */
            res += lines + lines;          /* account for +p/q and -p/q */
        }
    }

    return res;
}
