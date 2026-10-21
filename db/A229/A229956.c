/*
 * A229956  a(n) is the number m such that f(sqrt(n)) is in the field
 *          Q(sqrt(m)), where f(x) is defined from the continued fraction x =
 *          [c(0), c(1), ... ] as [1/c(0), 1/c(1), ...].
 * https://oeis.org/A229956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the squarefree part of the discriminant of the quadratic
 *            equation satisfied by f(sqrt(n)). f(x) is defined as taking the
 *            continued fraction of x and replacing each partial quotient k by
 *            1/k. f(sqrt(n)) has a periodic continued fraction, and its
 *            minimal quadratic equation has discriminant D...
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 45, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/squarefree.h"

/* integer square root of a 64‑bit value */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;          /* hi > sqrt(2^64-1) */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t a0   = isqrt_u64(n64);

    /* perfect square – result is 1 */
    if (a0 * a0 == n64) return (seq_t)1;

    /* continued fraction data for sqrt(n) */
    uint64_t m = 0, d = 1;
    uint64_t a = a0;                     /* current partial quotient */

    /* matrix product for the period of the reciprocal CF */
    seq_t P = (seq_t)1, Q = (seq_t)0, R = (seq_t)0, S = (seq_t)1;

    for (;;) {
        /* next partial quotient of sqrt(n) */
        unsigned __int128 tmp1 = (unsigned __int128)d * a - m;
        m = (uint64_t)tmp1;
        unsigned __int128 tmp2 = (unsigned __int128)(n64 - (unsigned __int128)m * m) / d;
        d = (uint64_t)tmp2;
        a = (a0 + m) / d;

        /* multiply matrix for the reciprocal partial quotient 1/a */
        seq_t ai = (seq_t)a;
        seq_t newP = P + Q * ai;
        seq_t newQ = P * ai;
        seq_t newR = R + S * ai;
        seq_t newS = R * ai;

        P = newP; Q = newQ; R = newR; S = newS;

        if (a == 2 * a0) break;
    }

    /* discriminant of the quadratic equation: D = (P-S)^2 + 4*R*Q */
    seq_t diff = (P >= S) ? (P - S) : (S - P);
    seq_t D = diff * diff + (R * Q) * (seq_t)4;

    return squarefree(D);
}
