/*
 * A118878  Let T(S,Q) be the sequence obtaining by starting with S and
 *          repeatedly reversing the digits and adding Q to get the next term.
 *          a(n) is the number of steps for T(n,5) to reach a cycle, or -1 if
 *          no cycle is ever reached.
 * https://oeis.org/A118878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the preperiod of the iterated function f(x) =
 *            reverse_decimal(x) + 5 starting from x = n. Returns the number
 *            of steps until the first element of the eventual cycle. If no
 *            cycle is ever reached (detectable when a limit is exceeded),
 *            returns (seq_t)-1
 * Range:     u8 n <= 1, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_decimal.h"

static inline seq_t next_seq(seq_t x)
{
    return reverse_decimal(x) + (seq_t)5;
}

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    seq_t tortoise = next_seq(start);
    seq_t hare = next_seq(next_seq(start));
    const idx_t MAX_ITER = 1ULL << 20;          /* limit to avoid infinite loops */
    idx_t iter = 0;
    while (tortoise != hare)
    {
        if (++iter > MAX_ITER)                  /* no cycle detected within limit */
            return (seq_t)-1;
        tortoise = next_seq(tortoise);
        hare = next_seq(next_seq(hare));
    }

    /* Compute mu: number of steps before entering the cycle */
    seq_t tortoise2 = start;
    seq_t hare2 = tortoise;
    idx_t mu = 0;
    while (tortoise2 != hare2)
    {
        tortoise2 = next_seq(tortoise2);
        hare2 = next_seq(hare2);
        mu++;
    }
    return (seq_t)mu;
}
