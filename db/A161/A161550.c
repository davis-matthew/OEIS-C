/*
 * A161550  Largest prime <= n^2+n.
 * https://oeis.org/A161550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime <= n^2 + n
 * Range:     u8 n <= 15, u16 n <= 254, u32 n <= 613, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is at least 1 according to the sequence offset */
    if (n == 0) {           /* not used, but keep for safety */
        return 0;
    }

    seq_t m = (seq_t)n * (seq_t)n + (seq_t)n;   /* compute n^2 + n in seq_t */

    /* Search downward for the first prime. The loop stops at 2, which
       is the smallest prime and is guaranteed to be found for n>=1. */
    seq_t x = m;
    for (;;) {
        if (is_prime_big(x)) {
            return x;
        }
        if (x == 2) {          /* will be found above, but safe guard */
            return 2;
        }
        --x;
    }
}
