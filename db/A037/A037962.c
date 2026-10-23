/*
 * A037962  a(n) = n*(15*n^3 + 30*n^2 + 5*n - 2)*(n+4)!/5760.
 * https://oeis.org/A037962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*S(n+4,n) * =
 *            n!*(C(n+7,8)+22*C(n+6,8)+58*C(n+5,8)+24*C(n+4,8)) * (where
 *            C(k,r) is the binomial coefficient). * For n=0, a(0)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 binom8(idx_t k)
{
    if (k < 8) return 0;
    unsigned __int128 r = 1;
    int i;
    for (i = 1; i <= 8; ++i) {
        r *= (unsigned __int128)(k - (i - 1));
        r /= (unsigned __int128)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute the Stirling Part and the binomial sum */
    unsigned __int128 b128 = 0;
    b128 += binom8(n + 7);
    b128 += (unsigned __int128)22 * binom8(n + 6);
    b128 += (unsigned __int128)58 * binom8(n + 5);
    b128 += (unsigned __int128)24 * binom8(n + 4);

    seq_t b = (seq_t)b128;          /* store in seq_t, safe for 64‑bit build when small */

    /* compute n! */
    seq_t fact = 1;
    idx_t i;
    for (i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    return fact * b;
}
