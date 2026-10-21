/*
 * A053001  Largest prime < n^2.
 * https://oeis.org/A053001
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = largest prime < n^2
 * Range:     u8 n <= 16, u16 n <= 255, u32 n <= 319, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined offset, return 0 */
    seq_t sq = (seq_t)n * (seq_t)n; /* compute n^2 in seq_t */
    seq_t cand = sq - 1;           /* start search below n^2 */
    while (!is_prime_big(cand)) {
        cand = cand - 1;
    }
    return cand;
}
