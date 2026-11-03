/*
 * A096993  Function A062402(x) = sigma(phi(x)) is iterated with initial
 *          value=n. a(n) is the length of cycle into which the trajectory
 *          merges.
 * https://oeis.org/A096993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the length of the cycle reached by iterating f(x) =
 *            σ(φ(x)) starting from n. f(x) is applied repeatedly until a
 *            value repeats; the cycle length λ is returned
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9444 (2026-11-01)
 * Helpers:   totient_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/totient_idx.h"

static /*@strict@*/ /*@only@*/ idx_t sigma_func(idx_t n)
/* Computes the sum of divisors σ(n).  Works for all n ≥ 1. */
{
    if (n == 0) return 0;
    idx_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t term = 1;        /* (p^(e+1)-1)/(p-1) for this prime */
            idx_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (temp > 1) {
        result *= (1 + temp);        /* final prime factor pf^1 */
    }
    return result;
}

static /*@strict@*/ /*@only@*/ idx_t f_func(idx_t x)
/* Composite function f(x) = σ(φ(x)). */
{
    return sigma_func(totient_idx(x));
}

seq_t a(idx_t n)
{
    /* Floyd's cycle‑finding algorithm. */
    idx_t tortoise = n;
    idx_t hare = n;

    /* First phase: find a meeting point. */
    do {
        tortoise = f_func(tortoise);
        hare = f_func(f_func(hare));
    } while (tortoise != hare);

    /* Second phase: compute cycle length λ. */
    seq_t lambda = 1;
    hare = f_func(tortoise);
    while (hare != tortoise) {
        hare = f_func(hare);
        ++lambda;
    }
    return lambda;
}
