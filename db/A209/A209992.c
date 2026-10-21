/*
 * A209992  Number of 2 X 2 matrices with all elements in {1,2,...,n} and
 *          determinant in {0,1}.
 * https://oeis.org/A209992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # {2×2 matrices with entries in {1,…,n} and det ∈ {0,1}}
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   ceil_div_int64, egcd_idx, floor_div_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by counting matrices with det 0 via totients and det 1 via
   extended Euclid on coprime pairs. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ceil_div_int64.h"
#include "../utils/egcd_idx.h"
#include "../utils/floor_div_int64.h"

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* ---------- zeros: det = 0 ----------
       Count solutions to ad = bc. Determinant zero iff
       a/b = c/d as a rational fraction.  For each reduced
       fraction p/q (gcd=1) with max(p,q) = M, there are
       floor(n/M)^2 pairs (a,b) and (c,d) proportional to it.
       Number of reduced fractions with max=M is 2*phi(M) for M>1
       and 1 for M=1.                           */
    uint32_t *phi = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!phi) return 0; /* failure */
    for (uint32_t i = 0; i <= n; ++i) phi[i] = i;
    phi[0] = 0;
    if (n >= 1) phi[1] = 1;
    for (uint32_t i = 2; i <= n; ++i) {
        if (phi[i] == i) { /* prime */
            for (uint32_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }
    seq_t zero = (seq_t)n * (seq_t)n; /* M = 1 term: 1 * floor(n/1)^2 */
    for (uint32_t M = 2; M <= n; ++M) {
        seq_t cnt_frac = (seq_t)2 * (seq_t)phi[M];
        seq_t t = (seq_t)(n / M);
        zero += cnt_frac * t * t;
    }

    /* ---------- ones: det = 1 ----------
       Count solutions to a*d - b*c = 1.  
       For each coprime pair (a,b), extend Euclid
       to get a particular solution (c0,d0).  All
       solutions are (c,d) = (c0 + k*a, d0 + k*b).
       Count k such that 1 ≤ c,d ≤ n. */
    seq_t one = 0;
    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        for (idx_t b_val = 1; b_val <= n; ++b_val) {
            if (gcd_idx(a_val, b_val) != 1) continue;
            /* Find integers d0,c0 with a*d0 - b*c0 = 1 */
            idx_t x, y;
            egcd_idx(a_val, b_val, &x, &y);
            /* After egcd: a_val*x + b_val*y = 1 */
            idx_t d0 = x;          /* a*d0 - b*c0 = 1 */
            idx_t c0 = -y;         /* because a*(-y) + b*x = 1 => a*(-y) = 1 - b*x */
            /* Compute feasible k interval */
            int64_t kmin_c = (int64_t)ceil_div_int64(1 - c0, a_val);
            int64_t kmax_c = (int64_t)floor_div_int64((int64_t)n - c0, a_val);
            int64_t kmin_d = (int64_t)ceil_div_int64(1 - d0, b_val);
            int64_t kmax_d = (int64_t)floor_div_int64((int64_t)n - d0, b_val);
            int64_t k_low = kmin_c > kmin_d ? kmin_c : kmin_d;
            int64_t k_high = kmax_c < kmax_d ? kmax_c : kmax_d;
            if (k_high >= k_low) {
                one += (seq_t)(k_high - k_low + 1);
            }
        }
    }
    free(phi);
    return zero + one;
}
