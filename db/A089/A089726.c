/*
 * A089726  Smallest prime of the form nk+1, k > n.
 * https://oeis.org/A089726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime of the form n*k+1 with k > n
 * Range:     u8 n <= 12, u16 n <= 248, u32 n <= 182, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n >= 1 is assumed */
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t k  = nn + 1;            /* start with the smallest k > n */

    for ( ; ; ++k) {
        seq_t p = nn * k + 1;     /* candidate prime */
        if (is_prime_big(p)) {
            return p;
        }
    }
}
