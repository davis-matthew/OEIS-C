/*
 * A028597  Expansion of theta_3(z) * theta_3(8*z) + theta_2(z) * theta_2(8*z)
 *          in powers of q^(1/4).
 * https://oeis.org/A028597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n % 4 = 2 or 3 a(4k) = # {(m,n) ∈ ℤ² : m² + 8 n² =
 *            k} a(4k+1) = 4 * # {(a,b) ∈ ℕ² : a² + a + 8 b² + 8 b = k-2},
 *            (k≥2) a(0) = 1
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    uint64_t mod4 = n & 3ULL;            /* n % 4 */

    /* case n % 4 == 0 */
    if (mod4 == 0) {
        seq_t k = (seq_t)(n >> 2);       /* k = n / 4         */
        seq_t count = 0;

        /* upper bound for n0: 8*n0² ≤ k  →  n0 ≤ sqrt(k)  */
        idx_t max_n0 = (idx_t)floor_sqrt(k);
        for (idx_t n0 = 0; n0 <= max_n0; ++n0) {
            seq_t t1 = (seq_t)8 * (seq_t)n0 * (seq_t)n0;   /* 8 n0² */
            if (t1 > k) break;                           /* no further solutions */

            seq_t rem = k - t1;                          /* m² */
            seq_t sq  = floor_sqrt(rem);
            if (sq * sq == rem) {                       /* rem is a perfect square */
                if (rem == 0) {                         /* m = 0                */
                    if (n0 == 0)
                        count += 1;                      /* (0,0) only           */
                    else
                        count += 2;                      /* (0,±n0)              */
                } else {                                 /* m = ±sqrt(rem)       */
                    if (n0 == 0)
                        count += 2;                      /* (±m,0)               */
                    else
                        count += 4;                      /* (±m,±n0)             */
                }
            }
        }
        return count;
    }

    /* case n % 4 == 1 */
    if (mod4 == 1) {
        seq_t k = (seq_t)((n - 1) >> 2);     /* k = (n-1)/4      */
        if (k < 2) return 0;                 /* n = 1 or 5 yields 0 */

        seq_t target = k - 2;                 /* target = k-2     */
        seq_t count  = 0;

        /* upper bound for b: 8b²+8b ≤ target → b ≤ sqrt(target) */
        idx_t max_b = (idx_t)floor_sqrt(target);
        for (idx_t b = 0; b <= max_b; ++b) {
            seq_t t2 = (seq_t)8 * (seq_t)b * (seq_t)b + (seq_t)8 * b;  /* 8b²+8b */
            if (t2 > target) break;               /* further b are too large */

            seq_t r = target - t2;                /* r = a² + a      */
            seq_t D = (seq_t)1 + (seq_t)4 * r;     /* discriminant    */
            seq_t sqrtD = floor_sqrt(D);
            if (sqrtD * sqrtD == D && (sqrtD & 1ULL)) {   /* sqrtD odd ⇒ integer a */
                count += 1;
            }
        }
        return count * (seq_t)4;                  /* multiply by 4   */
    }

    /* cases n % 4 == 2 or 3 */
    return 0;
}
