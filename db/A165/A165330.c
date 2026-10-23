/*
 * A165330  Result of repeatedly replacing a number with the sum of the cubes
 *          of its digits until a fixed point or cycle is reached.
 * https://oeis.org/A165330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9992, u32 n <= 9992, u64 n <= 9992,
 *            u128 n <= 9992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is defined by iterating the function f(x)=sum of
 * cubes of its digits.  Starting from n, the next term is f(n);  this
 * is repeated until a fixed point or a cycle is reached.  The function
 * returns the first element of the eventual cycle (or the fixed
 * point). */
#include <stdbool.h>
#include <stddef.h>

static const idx_t _cube[10] = {
    (idx_t)0,  (idx_t)1,  (idx_t)8,  (idx_t)27,
    (idx_t)64, (idx_t)125, (idx_t)216, (idx_t)343,
    (idx_t)512, (idx_t)729
};

static inline idx_t _sum_cubes(idx_t v)
{
    idx_t s = 0;
    if (v == 0) return 0;
    while (v != 0) {
        int d = (int)(v % 10);
        s += _cube[d];
        v /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    idx_t cur = n;
    /* Keep a record of previously seen values so that we can
     * detect a cycle.  The cycle length for this function is
     * at most a few elements, so we can use a small static array. */
    idx_t seen[64];
    size_t seen_cnt = 0;

    for (;;) {
        idx_t nxt = _sum_cubes(cur);

        /* A fixed point. */
        if (nxt == cur)
            return (seq_t)nxt;

        /* If nxt has been seen before, we are in a cycle. */
        for (size_t i = 0; i < seen_cnt; ++i) {
            if (nxt == seen[i])
                return (seq_t)nxt;
        }

        /* Record current value and proceed. */
        seen[seen_cnt++] = cur;
        cur = nxt;
    }
}
