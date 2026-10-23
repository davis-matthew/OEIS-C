/*
 * A042238  Numerators of continued fraction convergents to sqrt(645).
 * https://oeis.org/A042238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A042238: Numerators of continued fraction convergents to sqrt(645).
 * Recurrence: a(0) = 25, a(n) = periodA[(n-1) mod 10] * a(n-1) + a(n-2).
 * periodA = {2,1,1,12,10,12,1,1,2,50} (repeat cyclically).
 */

#include <stdint.h>

static const int periodA[10] = {2, 1, 1, 12, 10, 12, 1, 1, 2, 50};

/*
 * Compute a(n) for any n >= 0.
 * The function works for arbitrary seq_t types, provided seq_t
 * supports construction from int, +, and * operators.
 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)25;                  /* base case a(0) = 25 */

    /* a(-1) = 1, a(0) = 25 */
    seq_t prevprev = (seq_t)1;            /* a(-1) */
    seq_t prev    = (seq_t)25;            /* a(0) */
    seq_t cur;                           /* a(i) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = periodA[(i - 1) % 10];  /* int → seq_t implicit */
        cur = factor * prev + prevprev;        /* recurrence */
        prevprev = prev;
        prev = cur;
    }

    return cur;
}
