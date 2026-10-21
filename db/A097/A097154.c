/*
 * A097154  Number of iterations of f(x) = reverse(x) - maxdigit(x) needed to
 *          reach zero, or -1 if zero is never reached, where maxdig(n) is the
 *          largest digit of n.
 * https://oeis.org/A097154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 101, i16 n <= 127, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   maxdigit_seq_t, reverse_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A097154:
   a(n) = the number of iterations of f(x) = reverse(x) - maxdigit(x)
   needed to reach 0; -1 if 0 is never reached.
   The sequence is defined for n ≥ 1 (a(1) is the first term).  */

#include <stddef.h>
#include "../utils/maxdigit_seq_t.h"
#include "../utils/reverse_seq_t.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but safe */

    seq_t x = (seq_t)n;
    seq_t steps = (seq_t)0;

    /* Keep a small list of already visited values to detect cycles. */
    enum { MAX_STEPS = 10000 };
    static seq_t visited[MAX_STEPS];
    size_t visited_cnt = 0;

    while (1) {
        seq_t rev = reverse_seq_t(x);
        int maxd = maxdigit_seq_t(x);
        seq_t y = rev - (seq_t)maxd;

        steps++;

        if (y == (seq_t)0) return steps;

        /* Detect a cycle: if y has been seen before, it will never reach 0. */
        for (size_t i = 0; i < visited_cnt; ++i) {
            if (y == visited[i]) return (seq_t)-1;
        }

        if (visited_cnt < MAX_STEPS) {
            visited[visited_cnt++] = y;
        } else {
            /* Too many iterations – treat as non‑terminating. */
            return (seq_t)-1;
        }

        x = y;
    }
}
