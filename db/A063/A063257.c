/*
 * A063257  Let f be a function on rationals p/q (p,q coprime) defined by
 *          f(p/q) = abs(p-q)/g(p), where g(p) is the next odd number
 *          (starting with p) that we get after iteration of h(n) = n/2 when n
 *          is even, 5n+1 when n is odd. Start with f(n/1) and iterate f until
 *          it reaches again an integer, which is a(n)....
 * https://oeis.org/A063257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm:  compute A063257 by iterating f(p/q) =
 *     f(p/q) = |p-q| / g(p)
 * where g(p) is the first odd number obtained by iterating
 * h(n) = n/2  (n even)  or  5n+1 (n odd) starting from p,
 * at least one iteration.  Starting from n/1 the iteration is
 * repeated until the denominator becomes 1 (integer reached),
 * or until a loop appears or the numerator becomes 0.
 * The rational is reduced to coprime form after each step.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

#define MAX_STEPS 100000

static seq_t next_odd(seq_t p)
{
    seq_t n = p;
    while (1) {
        n = (n & 1) ? (5 * n + 1) : (n >> 1);
        if (n & 1) return n;            /* first odd after at least one step */
    }
}

seq_t a(idx_t n)
{
    seq_t p = (seq_t)n;
    seq_t q = (seq_t)1;

    /* arrays to detect cycles (coprime fractions) */
    static seq_t seen_p[MAX_STEPS];
    static seq_t seen_q[MAX_STEPS];
    size_t seen_cnt = 0;

    while (1) {
        /* cycle detection */
        for (size_t i = 0; i < seen_cnt; ++i) {
            if (seen_p[i] == p && seen_q[i] == q)
                return (seq_t)0;        /* cycle -> no integer */
        }
        if (seen_cnt >= MAX_STEPS) return (seq_t)0;

        seen_p[seen_cnt] = p;
        seen_q[seen_cnt] = q;
        ++seen_cnt;

        /* compute next fraction */
        seq_t diff = (p > q) ? (p - q) : (q - p);
        seq_t next_p = diff;
        seq_t next_q = next_odd(p);

        /* reduce to coprime form */
        seq_t g = gcd(next_p, next_q);
        if (g != 0) {
            next_p /= g;
            next_q /= g;
        }

        if (next_q == (seq_t)1)        /* integer reached */
            return next_p;

        /* if numerator becomes 0, integer 0 is reached */
        if (next_p == (seq_t)0)
            return (seq_t)0;

        p = next_p;
        q = next_q;
    }
}
