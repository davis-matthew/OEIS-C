/*
 * A054704  Number of powers of 3 modulo n.
 * https://oeis.org/A054704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct values of 3^k modulo n. Implemented by
 *            Floyd's cycle‑finding algorithm: a(n) = μ + λ where μ is the
 *            length of the transient part and λ is the length of the cycle.
 *            This works for all n ≥ 1
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   step_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/step_mod3.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1. */
    if (n <= 1)
        return 1;

    seq_t mod = (seq_t)n;

    /* Floyd's cycle‑finding. */
    seq_t tortoise = step_mod3(1, mod);                          /* x1 = f(x0)   */
    seq_t hare     = step_mod3(step_mod3(1, mod), mod);          /* x2 = f(f(x0))*/

    while (tortoise != hare) {
        tortoise = step_mod3(tortoise, mod);
        hare     = step_mod3(step_mod3(hare, mod), mod);
    }

    /* Find μ (length of non‑repeating part). */
    idx_t mu = 0;
    tortoise = 1; /* x0 */
    while (tortoise != hare) {
        tortoise = step_mod3(tortoise, mod);
        hare     = step_mod3(hare, mod);
        ++mu;
    }

    /* Find λ (length of cycle). */
    idx_t lambda = 1;
    hare = step_mod3(tortoise, mod);
    while (tortoise != hare) {
        hare = step_mod3(hare, mod);
        ++lambda;
    }

    return (seq_t)(mu + lambda);
}
