/*
 * A225015  Number of sawtooth patterns of length 1 in all Dyck paths of
 *          semilength n.
 * https://oeis.org/A225015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=1. For n>=3: a(n) = C_{n-2} * (9n-12) /
 *            (2n), where C_k = binom(2k, k) (central binomial coefficient)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   central_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/central_binom.h"

/* Computes the greatest‑common divisor of two seq_t values. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    seq_t C      = central_binom(n - 2);            /* C_{n-2}    */
    seq_t factor = (seq_t)9 * (seq_t)n - (seq_t)12;/* 9n-12      */
    seq_t denom  = (seq_t)2 * (seq_t)n;             /* 2n         */

    /* Compute res = C * factor / denom safely. */
    seq_t q = C / denom;
    seq_t r = C % denom;
    seq_t val = q * factor + (r * factor) / denom;
    return val;
}
