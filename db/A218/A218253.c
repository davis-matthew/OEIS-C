/*
 * A218253  The number of steps it takes, by taking the smallest number so
 *          that it minus the number of ones in its binary representation is
 *          the previous number, to reach a number which has no others that
 *          satisfy this property.
 * https://oeis.org/A218253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of steps following the smallest predecessor rule
 *            until a sink is reached
 * Range:     u8 n <= 519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: popcount for values that fit in 64 bits (sufficient for tests) */
static inline unsigned popcount_seq(seq_t x)
{
    return __builtin_popcountll((uint64_t)x);
}

/* find the smallest m > curr such that m - popcount(m) == curr.
   If no such m exists (curr is a sink), return 0. */
static seq_t find_pred(seq_t curr)
{
    /* If curr is the maximum value, no larger number exists */
    if (curr == (seq_t)-1) return 0;
    seq_t m = curr + 1;
    if (m <= curr) return 0;               /* overflow check */

    for (;;) {
        unsigned pc = popcount_seq(m);
        seq_t g     = m - (seq_t)pc;
        if (g == curr) return m;            /* predecessor found */
        if (g >  curr) break;               /* passed desired value */

        seq_t next = m + 1;
        if (next <= m) break;               /* overflow guard */
        m = next;
    }
    return 0;                               /* no predecessor */
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t cur  = (seq_t)n;
    idx_t steps = 0;

    for (;;) {
        seq_t pred = find_pred(cur);
        if (pred == 0) break;               /* sink reached */
        cur = pred;
        steps++;
    }
    return (seq_t)steps;
}
