/*
 * A106573  Perfect squares which are neither the sum nor the difference of
 *          two primes.
 * https://oeis.org/A106573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth odd square s such that s-2 and s+2 are both
 *            composite. This is equivalent to s not being expressible as the
 *            sum (2 and s-2) or the difference (2 and s+2) of two primes
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    idx_t count = 0;
    idx_t k = 0;                          /* iterate over odd numbers 2k+1 */

    while (count < n) {
        seq_t odd = (seq_t)(2 * k + 1);
        seq_t s   = odd * odd;           /* odd square */

        /* test both conditions: s-2 and s+2 must not be prime           */
        uint64_t s_minus_2 = (uint64_t)(s - 2);
        uint64_t s_plus_2  = (uint64_t)(s + 2);

        if (!is_prime(s_minus_2) && !is_prime(s_plus_2)) {
            ++count;
            if (count == n)
                return s;
        }
        ++k;
    }
    return 0;   /* should never be reached for valid n */
}
