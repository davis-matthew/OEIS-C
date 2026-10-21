/*
 * A097027  Function f(x) = phi(x) + floor(x/2) is iterated; a(n) is the
 *          length of transient if the iteration was initiated at n.
 * https://oeis.org/A097027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the transient part of iterating f(x) = φ(x) +
 *            ⎣x/2⎦, starting from x = n
 * Range:     u8 n <= 38, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use Floyd’s cycle‑finding algorithm: first find a point
   where the tortoise and hare meet, then advance one of them
   from the start until the two values coincide again to
   determine the tail length μ, which is the transient length. */

#include <stddef.h>
#include "../utils/phi.h"

static inline seq_t f(seq_t x)
{
    /* φ(x) + ⎣x/2⎦, all operations in seq_t (unsigned). */
    return phi((idx_t)x) + (x >> 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is defined for n ≥ 1 */
    const size_t MAX_ITER = 1000000;  /* safety limit for pathological cases */

    seq_t x0 = (seq_t)n;

    /* --- Phase 1: find a repetition using Floyd's algorithm --- */
    seq_t tortoise = f(x0);
    seq_t hare     = f(f(x0));

    size_t iter = 0;
    while (tortoise != hare) {
        if (++iter > MAX_ITER) return 0;   /* give up if too long */
        tortoise = f(tortoise);
        hare     = f(f(hare));
    }

    /* --- Phase 2: find the start of the cycle (μ) --- */
    idx_t mu = 0;
    tortoise = x0;
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare     = f(hare);
        ++mu;
    }

    return (seq_t)mu;
}
