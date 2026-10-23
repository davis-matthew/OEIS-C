/*
 * A118522  Define sequence S_n by: initial term = n, reverse digits and add 3
 *          to get next term. It is conjectured that S_n always reaches a
 *          cycle. Sequence gives number of steps for S_n to reach a cycle.
 * https://oeis.org/A118522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of steps until the iterative process S_{k+1} =
 *            reverse_decimal(S_k) + 3 reaches a cycle. a(n) = 0 if n is
 *            already in a cycle
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

/* Return the next value in the sequence S_k */
static inline seq_t step(seq_t x)
{
    /* reverse decimal digits then add 3 */
    return reverse_decimal(x) + (seq_t)3;
}

/* Compute the length of the preperiod (steps to first cycle entry) */
static seq_t preperiod_length(seq_t start)
{
    /* Floyd's cycle‑finding algorithm */
    seq_t tortoise = step(start);          /* f(x) */
    seq_t hare = step(step(start));        /* f(f(x)) */

    /* Phase 1: find meeting point inside the cycle */
    while (tortoise != hare) {
        tortoise = step(tortoise);
        hare = step(step(hare));
    }

    /* Phase 2: find start of cycle (mu) */
    seq_t mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = step(tortoise);
        hare = step(hare);
        mu += (seq_t)1;
    }

    return mu;
}

/* Main entry point for the OEIS sequence */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    return preperiod_length(start);
}
