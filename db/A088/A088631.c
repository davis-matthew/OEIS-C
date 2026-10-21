/*
 * A088631  Largest number m < n such that m+n is a prime.
 * https://oeis.org/A088631
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime ≤ 2*n‑1 minus n
 * Range:     u8 n <= 198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* offset is 2 */
    seq_t limit = (seq_t)n * 2 - 1;           /* 2*n - 1 */
    seq_t p = limit;
    while (!is_prime_big(p)) {                /* find largest prime ≤ limit */
        p--;
    }
    return p - (seq_t)n;
}
