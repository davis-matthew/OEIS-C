/*
 * A053471  a(n) is the cototient of n (A051953) iterated 3 times.
 * https://oeis.org/A053471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - φ(n)) - φ(n - φ(n)) - φ((n - φ(n)) - φ(n - φ(n)))
 * Range:     u8 n <= 641, u16 n <= 9456, u32 n <= 9456, u64 n <= 9456,
 *            u128 n <= 9456, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterating the cototient (c(n) = n - φ(n)) three times. */
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;          /* start with the index itself */
    for (int i = 0; i < 3; ++i) {
        if (cur == 0)             /* cototient(0) is defined as 0 */
            break;
        cur = cur - phi((idx_t)cur);
    }
    return cur;
}
