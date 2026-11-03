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
 * Range:     u8 n <= 63, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 121 terms through n = 9803 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static idx_t next_x(idx_t x)
{
    seq_t sigma   = sigma_of(x);   /* σ(x) */
    seq_t phi_sigma = totient_seq(sigma); /* φ(σ(x)) */
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
