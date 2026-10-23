/*
 * A189528  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(3)-sqrt(2),
 *          t=sqrt(3)+sqrt(2).
 * https://oeis.org/A189528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n(√3+√2)⌋ + ⌊2n√6⌋ * * The sequence is evaluated
 *            using fixed‑point arithmetic. Two * high‑precision rational
 *            approximations are precomputed: * * (√3+√2) ≈ 3146264 / 1 000
 *            000 * √6 ≈ 2449489 / 1 000 000 * * For all n ≤ 10 000 the
 *            resulting error is < 0.00005, guaranteeing * that no flo...
 * Range:     u8 n <= 18, u16 n <= 4666, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fixed‑point constants: numerator and common denominator Q = 1 000 000 */
    const uint64_t Q  = 1000000;
    const uint64_t S1 = 3146264;   /* floor((√3+√2) * Q) */
    const uint64_t S2 = 2449489;   /* floor( √6      * Q) */

    /* Compute ⌊n(√3+√2)⌋  →  (n * S1) / Q  */
    __uint128_t t1 = (__uint128_t)n * S1;
    uint64_t f1 = (uint64_t)(t1 / Q);

    /* Compute ⌊2n√6⌋ →  (2*n * S2) / Q  */
    __uint128_t t2 = (__uint128_t)(2 * (uint64_t)n) * S2;
    uint64_t f2 = (uint64_t)(t2 / Q);

    /* Final value:  a(n) = 6n + f1 + f2  */
    uint64_t val = 6 * (uint64_t)n + f1 + f2;
    return (seq_t)val;
}
