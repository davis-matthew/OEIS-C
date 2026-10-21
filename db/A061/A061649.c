/*
 * A061649  Smallest absolute value of a remainder when the larger of n and
 *          its reverse is divided by the smaller.
 * https://oeis.org/A061649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   * a(n) = | L - R * round(L / R) | with * L = max(n, rev(n)), R =
 *            min(n, rev(n)), * round(x) = nearest integer to x (ties rounded
 *            up). * If n == 0 return 0
 * Range:     u8 n <= 142, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/rev.h"

seq_t a(idx_t n) {
    /* Handle trivial case n == 0 */
    if (n == 0) return (seq_t)0;

    /* Compute reverse of n */
    idx_t rev_n = rev(n);

    /* If n and its reverse are equal, remainder is zero */
    if (rev_n == n) return (seq_t)0;

    /* Determine larger and smaller values (as seq_t) */
    seq_t big   = (seq_t)(n > rev_n ? n : rev_n);
    seq_t small = (seq_t)(n < rev_n ? n : rev_n);

    /* Compute the nearest integer quotient:
     * q = floor((big + small/2) / small)
     * Use a wider type to avoid overflow. */
    unsigned __int128 num = (unsigned __int128)big + ((unsigned __int128)small >> 1);
    seq_t q = (seq_t)(num / (unsigned __int128)small);

    /* Compute remainder that may be negative */
    signed __int128 rem = (signed __int128)big - (signed __int128)small * (signed __int128)q;

    /* Return its absolute value */
    if (rem < 0) rem = -rem;
    return (seq_t)rem;
}
