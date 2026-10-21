/*
 * A234868  a(n) = 2*binomial(11*n+2,n)/(11*n+2).
 * https://oeis.org/A234868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*binomial(11*n+2,n)/(11*n+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

#ifdef __cplusplus
/* C++ builds use the arbitrary‑precision binomial routine. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t denom = 11*n + 2;               /* 11*n+2 */
    seq_t val   = binomial(denom, n);     /* binomial(11*n+2,n) */
    val = val * (seq_t)2;                 /* multiply by 2 */
    val = val / (seq_t)denom;             /* divide by (11*n+2) */
    return val;
}
#else
/* C builds use 128‑bit arithmetic to avoid overflow for n≤15. */
static unsigned __int128 gcd_uint128(unsigned __int128 a, unsigned __int128 b)
{
    while (b) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t denom = 11*n + 2;                       /* 11*n+2 */

    unsigned __int128 num = 1, den = 1;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 num_factor = (unsigned __int128)(denom - k + 1);
        unsigned __int128 den_factor = (unsigned __int128)k;

        num *= num_factor;
        den *= den_factor;

        unsigned __int128 g = gcd_uint128(num, den);
        num /= g;
        den /= g;
    }

    num *= 2;          /* multiply by 2 */
    den *= denom;      /* multiply denominator by (11*n+2) */

    unsigned __int128 g = gcd_uint128(num, den);
    num /= g;
    den /= g;          /* den should now be 1 for an exact division */

    return (seq_t)(num / den);
}
#endif
