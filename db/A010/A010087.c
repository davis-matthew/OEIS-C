/*
 * A010087  Weight distribution of d=3 Hamming code of length 63.
 * https://oeis.org/A010087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (binomial(63,n-1) - a(n-1) - (63-n+2)*a(n-2))/n for n>1,
 *            a(0)=1, a(1)=0. Only weights up to 63 are possible; a(n)=0 for
 *            n>63
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The code uses only `seq_t` and `idx_t`; all intermediate
       computations are done in terms of these types. */
    static seq_t cache[64];
    static bool ready = false;
    const idx_t M = 63;          /* code length */

    if (!ready) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;

        /* compute binomial coefficients on the fly, avoiding intermediate
           overflow by reducing fractions using `gcd` from utils/gcd.h. */
        for (idx_t i = 2; i <= M; ++i) {
            /* binomial(M, i-1) */
            seq_t bm = (seq_t)1;
            for (idx_t j = 1; j <= i-1; ++j) {
                seq_t num = (seq_t)(M - j + 1);
                seq_t den = (seq_t)j;

                seq_t g = gcd(num, den);
                num /= g;
                den /= g;

                seq_t g2 = gcd(bm, den);
                bm /= g2;
                den /= g2;              /* den should be 1 now */

                bm *= num;              /* safe multiplication */
            }

            seq_t term   = ((seq_t)(M - i + 2)) * cache[i - 2];
            seq_t numer  = bm - cache[i - 1] - term;
            cache[i]    = numer / (seq_t)i;
        }
        ready = true;
    }

    if (n <= M) return cache[n];
    return (seq_t)0;
}
