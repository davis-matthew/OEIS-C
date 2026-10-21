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
 * Range:     u8 n <= 9444, u16 n <= 9444, u32 n <= 9444, u64 n <= 9444,
 *            u128 n <= 9444, big any n
 * Helpers:   phi_func, sigma_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_func.h"
#include "../utils/sigma_func.h"

static /*@strict@*/ /*@only@*/ idx_t f_func(idx_t x)
/* Composite function f(x) = σ(φ(x)). */
{
    return sigma_func(phi_func(x));
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
