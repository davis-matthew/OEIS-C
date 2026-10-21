/*
 * A088382  Numbers not exceeding the 4th power of their smallest prime
 *          factor.
 * https://oeis.org/A088382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that m <= lpf(m)^4 (lpf
 *            is the least prime factor of m)
 * Range:     u8 n <= 17, u16 n <= 459, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t count = 1;                  /* term 1 already counted */
    uint64_t candidate = 2u;          /* next integer to test */

    while (count < n) {
        uint64_t spf = _smallest_prime_factor(candidate);
        seq_t p4   = pow_int((seq_t)spf, (idx_t)4);
        seq_t val  = (seq_t)candidate;

        if (val <= p4) {              /* satisfies the condition */
            ++count;
            if (count == n)
                return val;
        }
        ++candidate;
    }
    /* unreachable: function must return a value for all n ≥ 1 */
    return (seq_t)0;
}
