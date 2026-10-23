/*
 * A033733  Expansion of Product_{d | 21} theta_3(q^d).
 * https://oeis.org/A033733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficient a(n) is the number of integer 4‑tuples
   (x1,x2,x3,x4) such that
          x1^2 + 3·x2^2 + 7·x3^2 + 21·x4^2 = n,
   but each non‑zero entry contributes a factor 2
   (because θ3(q) = Σ_{k∈ℤ} q^{k²} = 1+2·q^{1²}+2·q^{2²}+…).  */

/* Helper: integer square root for 64‑bit unsigned integers. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, mid, res = 0;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t x4 = 0;; ++x4) {
        __int128 t4 = (__int128)21 * x4 * x4;
        if (t4 > n) break;
        idx_t coeff4 = (x4 == 0 ? 1 : 2);

        for (idx_t x3 = 0;; ++x3) {
            __int128 t3 = (__int128)7 * x3 * x3 + t4;
            if (t3 > n) break;
            idx_t coeff3 = (x3 == 0 ? 1 : 2);

            for (idx_t x2 = 0;; ++x2) {
                __int128 t2 = (__int128)3 * x2 * x2 + t3;
                if (t2 > n) break;
                idx_t coeff2 = (x2 == 0 ? 1 : 2);

                idx_t rem = (idx_t)(n - t2);
                if (rem == 0) {
                    // x1 must be 0, coeff1 = 1
                    total += coeff2 * coeff3 * coeff4;
                } else {
                    idx_t m = isqrt_u64(rem);
                    if ((__int128)m * m == rem) {
                        idx_t coeff1 = (m == 0 ? 1 : 2);
                        total += coeff1 * coeff2 * coeff3 * coeff4;
                    }
                }
            }
        }
    }
    return total;
}
