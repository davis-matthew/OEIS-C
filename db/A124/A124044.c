/*
 * A124044  Number of ways to express 4n+3 as the sum of an odd square and
 *          twice a prime.
 * https://oeis.org/A124044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of representations of 4*n+3 as an odd square plus
 *            twice a prime
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* target value: 4*n + 3 */
    seq_t target = (seq_t)n * ((seq_t)4) + 3;

    seq_t count = 0;

    for (idx_t m = 1; ; m += 2) {          /* iterate over odd square roots */
        seq_t msq = (seq_t)m * m;
        if (msq > target) break;
        seq_t s = target - msq;
        if (s % 2 != 0) continue;           /* remainder must be even */
        seq_t p = s / 2;
        if (is_prime_big(p)) ++count;
    }

    return count;
}
