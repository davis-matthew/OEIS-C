/*
 * A128309  a(n) = 2*A000069(n).
 * https://oeis.org/A128309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A000069(n) (even odious numbers)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcnt_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcnt_parity.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  Return the n‑th even odious number.  */
    seq_t val   = 2;    /* start with the first even candidate */
    idx_t count = 0;    /* how many even odious numbers found so far */

    while (count < n) {
        if (popcnt_parity(val)) {
            count++;
            if (count == n)
                return val; /* found the n‑th entry */
        }
        val += 2;    /* advance to the next even number */
    }

    /* unreachable, but keep the compiler happy */
    return 0;
}
