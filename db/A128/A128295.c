/*
 * A128295  a(n) = numerator of b(n): b(1)=1; b(n+1) = b(n) *
 *          [b(1);b(2),...,b(n)], where [...] is a continued fraction of
 *          rational terms.
 * https://oeis.org/A128295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A128295: a(n) = numerator of b(n)
   b(1) = 1
   b(n+1) = b(n) * [b(1);b(2),...,b(n)]   (continued fraction with rational terms) */

#include "../utils/gcd.h"
#include <stdlib.h>

/* multiply two quotients a/b and c/d with cross‑cancellation to avoid overflow.
   Both a,b,c,d are already in lowest terms. */
static inline seq_t mul_frac(seq_t a, seq_t b, seq_t c, seq_t d)
{
    seq_t g1 = gcd(a, d);
    seq_t g2 = gcd(c, b);
    a   /= g1; d /= g1;
    c   /= g2; b /= g2;
    return (seq_t)((__uint128_t)a * c) / (seq_t)((__uint128_t)b * d);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 – value for n<1 irrelevant */
    if (n == 1) return 1;

    /* storage for b(1)..b(n) as numerators and denominators */
    seq_t *num = (seq_t *)malloc(n * sizeof(seq_t));
    seq_t *den = (seq_t *)malloc(n * sizeof(seq_t));
    if (!num || !den) return 0;    /* allocation failure – caller should handle */

    num[0] = 1;   /* b(1) = 1/1  */
    den[0] = 1;

    for (idx_t i = 1; i < n; ++i)        /* compute b(i+1) from b(1..i) */
    {
        /* evaluate the continued fraction C_i = [b(1);...;b(i)] */
        seq_t cf_num = num[i-1];
        seq_t cf_den = den[i-1];
        for (idx_t k = i-2; (idx_t)k != (idx_t)(-1); --k)   /* walk backwards */
        {
            seq_t a = num[k];
            seq_t bden = den[k];
            seq_t p = cf_num;
            seq_t q = cf_den;

            seq_t new_num = a * p + bden * q;       /* (a/p) + (q/p)  */
            seq_t new_den = bden * p;
            seq_t g = gcd(new_num, new_den);
            cf_num = new_num / g;
            cf_den = new_den / g;
        }

        /* b(i+1) = b(i) * cf_num/cf_den   */
        seq_t g1 = gcd(num[i-1], cf_den);
        seq_t g2 = gcd(cf_num, den[i-1]);

        seq_t nnum = (num[i-1]/g1) * (cf_num/g2);
        seq_t nden = (den[i-1]/g2) * (cf_den/g1);

        seq_t g3 = gcd(nnum, nden);
        num[i] = nnum / g3;
        den[i] = nden / g3;
    }

    seq_t ans = num[n-1];         /* numerator of b(n) */
    free(num);
    free(den);
    return ans;
}
