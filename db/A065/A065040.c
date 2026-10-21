/*
 * A065040  Triangle read by rows: T(m,k) = exponent of the highest power of 2
 *          dividing the binomial coefficient binomial(m,k).
 * https://oeis.org/A065040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = v2( binomial( m , k ) ) where m =
 *            floor((1+sqrt(8*n+1))/2)-1 and k = n - m*(m+1)/2 v2(x) is the
 *            exponent of the highest power of 2 dividing x. Use the identity
 *            v2(binomial(m,k)) = popcount(k) + popcount(m-k) - popcount(m)
 * Range:     u8 n <= 9512, u16 n <= 9512, u32 n <= 9512, u64 n <= 9512,
 *            u128 n <= 9512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 isqrt128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    /* iterate bits from high to low; 63 suffices because sqrt(x) < 2^64 */
    for (int i = 63; i >= 0; --i) {
        unsigned __int128 tmp = r | ((unsigned __int128)1 << i);
        if (tmp * tmp <= x)
            r = tmp;
    }
    return r;
}

static inline int popcnt64(uint64_t v) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(v);
#else
    /* Fallback simple algorithm if builtin not available */
    int c = 0;
    while (v) {
        v &= v - 1;
        ++c;
    }
    return c;
#endif
}

extern seq_t a(idx_t n) {
    /* Compute m = floor((1+sqrt(8*n+1))/2)-1 */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    unsigned __int128 d   = isqrt128(val);        /* floor sqrt(8n+1) */
    unsigned __int128 s   = (d + 1) / 2;          /* floor((1+sqrt)/2) */
    uint64_t m = (uint64_t)(s - 1);               /* row index */

    /* Compute k = n - m*(m+1)/2 */
    unsigned __int128 t = (unsigned __int128)m * (m + 1) / 2;
    uint64_t k = (uint64_t)(n - (idx_t)t);

    int v = popcnt64(k) + popcnt64(m - k) - popcnt64(m);
    return (seq_t)v; /* v is always non‑negative and fits in seq_t */
}
