/*
 * A119971  G.f. sum{k>=0, (x^2/(1-x)^3)^(2^k-1)}.
 * https://oeis.org/A119971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k>=0, 2^k-1 <= n/2} C(n + 2^k-1 - 1, n - 2*(2^k-1))
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(a,b) is the binomial coefficient a choose b.  This follows from
   the generating function Σ_{k≥0} (x²/(1−x)³)^(2^k−1). */

#include <stdint.h>
#include "../utils/gcd.h"

/* multiplicative binomial coefficient, works for all seq_t (over 64‑bit
   and arbitrary precision). */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)               return 0;
    if (k == 0 || k == n)    return 1;
    if (k > n - k)           k = n - k;   /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);   /* (n‑k+i) */
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);         /* cancel common factors with den */
        num /= g;  den /= g;
        g = gcd(res, den);               /* cancel remaining factors */
        res /= g;  den /= g;

        /* now ‘den’ should be 1; perform the multiplication/division */
        res = (res * num) / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* constant term from m = 0 */

    seq_t total = 0;
    for (idx_t k = 0; ; ++k) {
        idx_t m;                         /* m = 2^k – 1 */
        if (k == 0)                      m = 0;
        else {
            /* avoid overflow of 1ULL<<k when k is large */
            if (k >= sizeof(idx_t) * 8) break;
            m = ((idx_t)1 << k) - 1;
        }

        if ((idx_t)2 * m > n) break;      /* x‑power too high → term is 0 */

        idx_t top    = n + m - 1;          /* n + m - 1 */
        idx_t bottom = n - 2 * m;          /* n - 2m */
        total += binom(top, bottom);
    }
    return total;
}
