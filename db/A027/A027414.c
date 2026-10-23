/*
 * A027414  G.f. for Moebius transform is x * (1 + x) / (1 + x^4).
 * https://oeis.org/A027414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k|n, (n/k)≡1 or 2 (mod 4)} (−1)^((n/k−r)/4) where r =
 *            1 for (n/k)≡1 (mod 4), r = 2 for (n/k)≡2 (mod 4)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value a(n) is the coefficient of x^n in the generating function
   Σ_{k>0} x^k (1+x^k)/(1+x^{4k}), computed by summing over divisors of n.  */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* safety for zero index */
    int64_t sum = 0;                       /* accumulator (signed) */

    for (idx_t k = 1; k <= n / k; ++k) {    /* iterate divisors up to sqrt(n) */
        if (n % k == 0) {
            /* first divisor: k */
            idx_t d = n / k;                 /* corresponding n/k */
            if ((d & 3) == 1) {              /* d % 4 == 1 */
                idx_t m = (d - 1) >> 2;      /* (d-1)/4 */
                sum += (int64_t)((m & 1) ? -1 : 1);
            }
            if ((d & 3) == 2) {              /* d % 4 == 2 */
                idx_t m = (d - 2) >> 2;      /* (d-2)/4 */
                sum += (int64_t)((m & 1) ? -1 : 1);
            }

            /* second divisor: d (if distinct from k) */
            if (d != k) {
                idx_t d2 = k;                 /* n/(n/k) == k */
                if ((d2 & 3) == 1) {
                    idx_t m = (d2 - 1) >> 2;
                    sum += (int64_t)((m & 1) ? -1 : 1);
                }
                if ((d2 & 3) == 2) {
                    idx_t m = (d2 - 2) >> 2;
                    sum += (int64_t)((m & 1) ? -1 : 1);
                }
            }
        }
    }
    return (seq_t)sum;                     /* all sequence values are non‑negative */
}
