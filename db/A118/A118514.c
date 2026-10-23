/*
 * A118514  Define sequence S_n by: initial term = n, reverse digits and add 2
 *          to get next term. It is conjectured that S_n always reaches a
 *          cycle. Sequence gives number of steps for S_n to reach a cycle.
 * https://oeis.org/A118514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"
/* Computation of steps to reach a cycle for sequence A118514.
   S0 = n,  Sj+1 = reverse_digits(Sj) + 2.
   a(n) = length of the pre‑period (μ) before the sequence enters its cycle. */

static inline seq_t step(seq_t x) {
    return reverse_digits(x) + 2;
}

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;

    /* Floyd's cycle‑finding algorithm. */
    seq_t tortoise = step(cur);
    seq_t hare     = step(step(cur));
    while (tortoise != hare) {
        tortoise = step(tortoise);
        hare     = step(step(hare));
    }

    /* Find entry point of the cycle. */
    tortoise = cur;
    seq_t mu = 0;
    while (tortoise != hare) {
        tortoise = step(tortoise);
        hare     = step(hare);
        mu++;
    }

    return mu;
}
