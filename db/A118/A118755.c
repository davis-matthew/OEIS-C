/*
 * A118755  Smallest prime >= 6*n.
 * https://oeis.org/A118755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime >= 6*n (A007918(A008588(n)))
 * Range:     u8 n <= 41, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C implementation for OEIS A118755 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)n * 6;          /* 6*n */
    if (p < (seq_t)2)                /* handle n = 0 */
        p = 2;
    while (!is_prime_big(p))         /* find first prime >= 6*n */
        ++p;
    return p;
}
