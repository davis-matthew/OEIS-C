/*
 * A092405  a(n) = tau(n) + tau(n+1), where tau(n) = A000005(n), the number of
 *          divisors of n.
 * https://oeis.org/A092405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + tau(n+1) where tau(n) is the number of positive
 *            divisors of n
 * Range:     u8 n <= 9402, u16 n <= 9402, u32 n <= 9402, u64 n <= 9402,
 *            u128 n <= 9402, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count divisors of a 64‑bit integer.  The result fits in 32 bits for all
   n up to 2^64, so we safely store it in a uint64_t. */
static uint64_t divisor_count(uint64_t m)
{
    uint64_t count = 1;
    uint64_t tmp   = m;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            count *= exp + 1;
        }
    }
    if (tmp > 1) /* remaining prime factor */
        count *= 2;

    return count;
}

/* Public sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    uint64_t n0      = (uint64_t)n;
    uint64_t tau_n   = divisor_count(n0);
    uint64_t tau_n1  = divisor_count(n0 + 1);
    return (seq_t)tau_n + (seq_t)tau_n1;
}
