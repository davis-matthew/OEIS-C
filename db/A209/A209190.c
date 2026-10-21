/*
 * A209190  Least prime factor of reversal of digits of n.
 * https://oeis.org/A209190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = lpf( rev(n) )
 * Range:     u8 n <= 103, u16 n <= 9775, u32 n <= 9775, u64 n <= 9775,
 *            u128 n <= 9775, big any n
 * Helpers:   lpf, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    /* Reverse the decimal representation of n. */
    idx_t r = rev(n);

    /* For r == 1 return 1, since 1 has no prime factor. */
    if (r <= (idx_t)1) {
        return (seq_t)1;
    }

    /* Least prime factor of the reversed number. */
    idx_t p = lpf(r);

    return (seq_t)p;
}
