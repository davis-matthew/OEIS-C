/*
 * A022248  Gaussian binomial coefficients [ n,8 ] for q = 8.
 * https://oeis.org/A022248
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..8} (8^(n-i+1)-1)/(8^i-1)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 8) return (seq_t)0;

    seq_t prev[9] = {0};
    seq_t cur[9];
    prev[0] = (seq_t)1;        /* G(0,0) = 1 */

    /* small powers of 8 (8^t for t = 0..7) */
    static const idx_t smallPow[8] = {
        1ULL,          /* 8^0 */
        8ULL,          /* 8^1 */
        64ULL,         /* 8^2 */
        512ULL,        /* 8^3 */
        4096ULL,       /* 8^4 */
        32768ULL,      /* 8^5 */
        262144ULL,     /* 8^6 */
        2097152ULL     /* 8^7 */
    };

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = (seq_t)1;   /* G(i,0) = 1 */

        if (i >= 8) {
            /* 8^(i-8) computed once per i */
            seq_t powBase = seq_pow((seq_t)8, i - 8);

            for (idx_t k = 1; k <= 8; ++k) {
                if (k > i) {          /* G(i,k) = 0 for k > i */
                    cur[k] = (seq_t)0;
                    continue;
                }
                idx_t t = 8 - k;                    /* exponent adjustment */
                seq_t powk = powBase * (seq_t)smallPow[t]; /* 8^(i-k) */
                seq_t term2 = prev[k - 1] * powk;
                cur[k] = prev[k] + term2;
            }
        } else {
            for (idx_t k = 1; k <= i; ++k) {
                idx_t t = i - k;                    /* 0 <= t <= 7 */
                seq_t powk = (seq_t)smallPow[t];     /* 8^(i-k) */
                seq_t term2 = prev[k - 1] * powk;
                cur[k] = prev[k] + term2;
            }
            for (idx_t k = i + 1; k <= 8; ++k)
                cur[k] = (seq_t)0;                   /* G(i,k)=0 for k>i */
        }

        /* copy cur into prev for next iteration */
        for (idx_t k = 0; k <= 8; ++k)
            prev[k] = cur[k];
    }

    return prev[8];
}
