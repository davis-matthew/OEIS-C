/*
 * A057081  Even-indexed Chebyshev U-polynomials evaluated at sqrt(11)/2.
 * https://oeis.org/A057081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - a(n-2), a(-1) = -1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t prev = (seq_t)1; /* a(0) */
    seq_t curr = (seq_t)10;/* a(1) */

    /* Iterate up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)9 * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
