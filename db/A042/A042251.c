/*
 * A042251  Denominators of continued fraction convergents to sqrt(651).
 * https://oeis.org/A042251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042251: denominators of continued‑fraction convergents to sqrt(651)
 * Recurrence: a(0) = 1,
 *              a(1) = 1,
 *              a(n) = c_n * a(n-1) + a(n-2)   (n ≥ 2),
 * where the partial quotients c_n repeat the period
 * [1, 1, 16, 1, 1, 50] (period 6).  This generates the sequence of
 * denominators of the convergents of sqrt(651) with a(0) as defined above.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued‑fraction expansion after the
     * integer part floor(sqrt(651)) = 25.  The first partial quotient
     * a₁ is the first element of this cycle. */
    const unsigned int cycle[6] = {1, 1, 16, 1, 1, 50};

    /* Fibonacci–like recurrence for convergent denominators.
     * q_-1 = 0, q_0 = 1,   a(0) = 1. */
    seq_t prev2 = 0;  /* q_{n-2} */
    seq_t prev1 = 1;  /* q_{n-1} */

    if (n == 0) {
        return prev1;          /* a(0) = 1 */
    }

    /* Compute q_n iteratively using the cycle of partial quotients. */
    for (idx_t i = 1; i <= n; ++i) {
        size_t idx = (size_t)((i - 1) % 6);
        unsigned int c = cycle[idx];
        seq_t cur = prev1 * (seq_t)c + prev2; /* q_n = a_n * q_{n-1} + q_{n-2} */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
