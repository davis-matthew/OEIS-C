/*
 * A088732  First prime in the arithmetic progression n+k*(n+1) with k>0.
 * https://oeis.org/A088732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first prime in the arithmetic progression n + k*(n+1)
 *            with k >= 1
 * Range:     u8 n <= 41, u16 n <= 1665, u32 n <= 6493, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t step  = (seq_t)n + 1;          /* n + 1, the step of the AP */
    seq_t term  = (seq_t)n + step;       /* start with k = 1 */

    /* search forward until we hit a prime */
    while (!is_prime_big(term)) {
        term += step;
    }
    return term;
}
