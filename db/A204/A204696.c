/*
 * A204696  G.f.: (32*x^7/(1-2*x) + 16*x^5 + 24*x^6)/(1-2*x^2).
 * https://oeis.org/A204696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) derived from generating function
 *            (32*x^7/(1-2*x)+16*x^5+24*x^6)/(1-2*x^2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0..4) = 0. For n>=5:
 * a(n) = (2^(n-1) - 2^(n-2-floor((n-7)/2)))                   if n>=7
 *       + (n odd >=5) 16*2^((n-5)/2)
 *       + (n even >=6) 24*2^((n-6)/2)                            */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 5) return (seq_t)0;
    seq_t result = (seq_t)0;

    /* Term A: from 32*x^7/(1-2*x) part */
    if (n >= 7) {
        idx_t k = (n - 7) / 2;                   /* floor((n-7)/2) */
        seq_t termA = pow2(n - 1) - pow2(n - 2 - k);
        result += termA;
    }

    /* Term B: from 16*x^5 part (only for odd n >=5) */
    if ((n >= 5) && (n & 1)) {
        idx_t j = (n - 5) / 2;
        result += pow2(j + 4);                   /* 16*2^j = 2^(j+4) */
    }

    /* Term C: from 24*x^6 part (only for even n >=6) */
    if ((n >= 6) && (!(n & 1))) {
        idx_t j = (n - 6) / 2;
        seq_t temp = pow2(j + 3);                /* 8*2^j */
        result += temp * (seq_t)3;               /* multiply by 3 to get 24*2^j */
    }

    return result;
}
