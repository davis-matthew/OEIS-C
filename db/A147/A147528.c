/*
 * A147528  Numbers x such that (x + 103)^3 - x^3 is a square.
 * https://oeis.org/A147528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A147528: Numbers x such that (x + 103)^3 - x^3 is a square.
   Recurrence: a(1) = 5327263
               a(2) = 684056220943393618
               a(n+2) = 128405450990*a(n+1) - a(n) + 6612880725882  for n ≥ 1
   This function computes the nth term purely iteratively, so it works
   for any order of calls and requires only O(1) additional memory. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle indices below the sequence offset (1). */
    if (n < 1)
        return (seq_t)0;

    /* Base values. */
    const seq_t a1 = (seq_t)5327263;
    const seq_t a2 = (seq_t)684056220943393618ULL;

    if (n == 1)
        return a1;
    if (n == 2)
        return a2;

    /* Recurrence constants. */
    const seq_t C = (seq_t)128405450990ULL;
    const seq_t D = (seq_t)6612880725882ULL;

    /* Iteratively compute terms up to n. */
    seq_t a_prev = a1;      /* a(n-1)   */
    seq_t a_curr = a2;      /* a(n)     */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        a_next = C * a_curr - a_prev + D;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
