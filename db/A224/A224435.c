/*
 * A224435  The Wiener index of the polyphenylene dendrimer G[n] defined
 *          pictorially in the N. E. Arif et al. reference.
 * https://oeis.org/A224435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -31176 + 136464*2^n + 93600*n*4^n - 31860*n*2^n -
 *            104040*4^n
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 21, u128 n <= 52,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 969 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);   /* 2^n  */
    seq_t pow4n = pow4(n);   /* 4^n  */

    seq_t res = (seq_t)-31176
                + (seq_t)136464 * pow2n
                + (seq_t)93600 * (seq_t)n * pow4n
                - (seq_t)31860 * (seq_t)n * pow2n
                - (seq_t)104040 * pow4n;

    return res;
}
