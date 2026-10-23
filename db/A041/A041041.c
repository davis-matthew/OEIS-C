/*
 * A041041  Denominators of continued fraction convergents to sqrt(26).
 * https://oeis.org/A041041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of OEIS sequence A041041 (denominators of convergents to sqrt(26)). */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* Iteratively compute the recurrence. */
    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)10;  /* a(1) */
    seq_t nxt;

    for (idx_t i = 2; i <= n; ++i) {
        nxt = (seq_t)10 * curr + prev;
        prev = curr;
        curr = nxt;
    }
    return curr;
}
