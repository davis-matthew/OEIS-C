/*
 * A108712  A fractal sequence, defined by a(2n-1) = A007376(n) (the
 *          almost-natural numbers), a(2n) = a(n).
 * https://oeis.org/A108712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(2n-1) = A007376(n); a(2n) = a(n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   A007376
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A007376.h"

seq_t a(idx_t n)
{
    /* Reduce even indices by dividing by 2 until the index becomes odd.
     * For an odd index n, we have n = 2k-1, so a(n) = A007376(k). */
    while ((n & 1) == 0) {
        n >>= 1;          /* n /= 2 */
    }
    return A007376((n + 1) / 2);
}
