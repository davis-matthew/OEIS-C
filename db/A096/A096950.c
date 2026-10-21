/*
 * A096950  Denominators of partial sums of series for 3*arctanh(1/3) =
 *          (3/2)*log(2).
 * https://oeis.org/A096950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of \sum_{k=0..n} (1/3)^(2k)/(2k+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

#ifdef __cplusplus
/* C++ (arbitrary‑precision) implementation */
seq_t a(idx_t n) {
    seq_t N = 0;
    seq_t D = 1;
    seq_t power3 = 1;

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) power3 *= 9;                     /* 3^(2k) */
        seq_t denom = power3 * (seq_t)(2 * k + 1);   /* 3^(2k)*(2k+1) */

        seq_t g1   = gcd(D, denom);
        seq_t D1   = D / g1;
        seq_t d1   = denom / g1;

        seq_t tempN = N * d1 + D1;
        seq_t tempD = D1 * denom;

        seq_t g2 = gcd(tempN, tempD);
        N = tempN / g2;
        D = tempD / g2;
    }
    return D;
}

#else
/* C (fixed‑width) implementation with 128‑bit intermediates */
static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b) {
    while (b) {
        unsigned __int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t N = 0;
    seq_t D = 1;
    seq_t power3 = 1;

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) power3 = power3 * 9;                         /* 3^(2k) */
        seq_t denom = power3 * (seq_t)(2 * k + 1);               /* 3^(2k)*(2k+1) */

        seq_t g1 = gcd(D, denom);
        seq_t D1 = D / g1;
        seq_t d1 = denom / g1;

        unsigned __int128 tempN = (unsigned __int128)N * (unsigned __int128)d1
                                 + (unsigned __int128)D1;
        unsigned __int128 tempD = (unsigned __int128)D1 * (unsigned __int128)denom;

        unsigned __int128 g2 = gcd128(tempN, tempD);

        N = (seq_t)(tempN / g2);
        D = (seq_t)(tempD / g2);
    }
    return D;
}
#endif
