/*
 * A061340  a(n) = n*omega(n)^n where omega(n) is the number of distinct prime
 *          divisors of n.
 * https://oeis.org/A061340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, changed
 * Formula:   a(n) = n * omega(n)^n where omega(n) = number of distinct prime
 *            divisors of n
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 41, u128 n <= 77,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

/* Fast exponentiation: compute base^exp using only seq_t arithmetic. */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Main function implementing a(n). */
seq_t a(uint64_t n)
{
    if (n == 1)      /* omega(1) = 0, 0^1 = 0 */
        return 0;

    int omega = count_distinct_primes(n);
    seq_t pw = pow_seq_t((seq_t)omega, n);
    return (seq_t)n * pw;
}
