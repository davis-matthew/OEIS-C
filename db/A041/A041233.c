/*
 * A041233  Denominators of continued fraction convergents to sqrt(128).
 * https://oeis.org/A041233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1154*a(n-4) - a(n-8) (n ≥ 8) Initial values: a(0) = 1,
 *            a(1) = 3, a(2) = 16, a(3) = 51, a(4) = 1138, a(5) = 3465, a(6) =
 *            18463, a(7) = 58854
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 192 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>               /* for uint64_t and the 128‑bit type */
#ifdef __cplusplus
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}
#endif

seq_t a(idx_t n)
{
    /* first eight terms from the OEIS entry */
    static const seq_t init[8] = {1, 3, 16, 51, 1138, 3465, 18463, 58854};

    if (n < 8)
        return init[n];

    /* buffer holding the last eight computed values */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i)
    {
        idx_t i4 = (i - 4) & 7;   /* (i-4) modulo 8 */
        idx_t i8 = (i - 8) & 7;   /* (i-8) modulo 8 */

#ifdef __cplusplus
        /* For arbitrary‑precision seq_t, use provided mul_small function */
        seq_t prod = mul_small(buf[i4], 1154) - buf[i8];
        buf[i & 7] = prod;
#else
        /* For built‑in integer types, use 128‑bit arithmetic to avoid overflow */
        seq_t cur = (seq_t)(((__uint128_t)1154 * buf[i4]) -
                            (unsigned __int128)buf[i8]);
        buf[i & 7] = cur;
#endif
    }

    return buf[n & 7];
}
