/*
 * A082991  a(1) = 1 and for n > 1, a(n) = 2 * length of the cycle reached for
 *          the map x -> A062401(x), starting at n [where A062401(n) =
 *          phi(sigma(n))], or -1 if no finite cycle is ever reached.
 * https://oeis.org/A082991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * length of cycle of x -> φ(σ(x)) starting at n (a(1) =
 *            1). φ(m) = Euler's totient, σ(m) = sum of divisors
 * Range:     u8 n <= 63, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

static idx_t next_x(idx_t x)
{
    seq_t sigma   = sigma_of(x);   /* σ(x) */
    seq_t phi_sigma = calc_phi(sigma); /* φ(σ(x)) */
    return (idx_t)phi_sigma;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Floyd's cycle‑finding algorithm with a hard stop to avoid
       infinite loops in pathological cases. */
    const size_t MAX_STEPS = 100000;

    idx_t tort = next_x(n);
    idx_t hare = next_x(next_x(n));
    size_t steps = 0;
    while (tort != hare) {
        if (++steps > MAX_STEPS)
            return (seq_t)-1;
        tort = next_x(tort);
        hare = next_x(next_x(hare));
    }

    /* Find start of cycle. */
    idx_t ptr   = n;
    idx_t start = hare;
    steps = 0;
    while (ptr != start) {
        if (++steps > MAX_STEPS)
            return (seq_t)-1;
        ptr   = next_x(ptr);
        start = next_x(start);
    }

    /* Determine cycle length λ. */
    size_t lambda = 1;
    idx_t cur = next_x(start);
    while (cur != start) {
        if (++lambda > MAX_STEPS)
            return (seq_t)-1;
        cur = next_x(cur);
    }

    /* a(n) = 2 · λ. */
    return (seq_t)(lambda << 1);
}
