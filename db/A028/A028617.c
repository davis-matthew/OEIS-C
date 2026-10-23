/*
 * A028617  Expansion of (theta_3(z)*theta_3(13z) + theta_2(z)*theta_2(13z)).
 * https://oeis.org/A028617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the coefficient of q^n in
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* theta_3(z)*theta_3(13z) + theta_2(z)*theta_2(13z).  */
/* For even n: let k = n/2.  Count all pairs (i,j) of non‑negative integers with */
/* i^2 + 13*j^2 = k.  Each i>0 contributes weight 2 (because +i and –i give the same exponent); */
/* each j>0 contributes weight 2.  The term contributes 2*2=4 to a(n); i=0 or j=0 contribute 1. */
 
/* For odd n: let t = (n-7)/2.  Count all pairs (x,y) of non‑negative integers with  */
/* x^2 + x + 13*(y^2 + y) = t.  Every such pair contributes 4 to a(n).  (This comes from the */
/* theta_2 part, after accounting for its factor 2 in each series.) */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* ----- theta_3 part (even indices) ----- */
    if ((n & 1) == 0) {                 /* even n */
        idx_t k = n / 2;
        /* loop i such that i^2 <= k */
        for (uint64_t i = 0;; ++i) {
            uint64_t ii = i * i;
            if (ii > k) break;
            seq_t w1 = (i == 0) ? (seq_t)1 : (seq_t)2;
            uint64_t v = (uint64_t)(k - ii);
            seq_t w2 = 0;
            if (v == 0) {
                w2 = 1;
            } else if (v % 13 == 0) {
                uint64_t s = v / 13;
                /* check if s is a perfect square */
                uint64_t sq = (uint64_t)(sqrt((long double)s) + 0.5);
                if (sq * sq == s)
                    w2 = 2;
            }
            if (w2 != 0)
                result += w1 * w2;
        }
    }

    /* ----- theta_2 part (odd indices) ----- */
    if (n >= 7 && ((n - 7) & 1) == 0) {   /* n satisfies n ≡ 7 (mod 2) and n ≥ 7 */
        idx_t target = (n - 7) / 2;
        for (uint64_t x = 0; ; ++x) {
            uint64_t xx = x * x + x;
            if (xx > target) break;
            uint64_t rem = target - xx;
            if (rem % 13 != 0) continue;
            uint64_t rem13 = rem / 13;
            /* solve y^2 + y = rem13 */
            for (uint64_t y = 0; ; ++y) {
                uint64_t yy = y * y + y;
                if (yy > rem13) break;
                if (yy == rem13) {
                    result += 4;   /* each pair contributes 4 */
                    break;         /* only one y per x */
                }
            }
        }
    }

    return result;
}
