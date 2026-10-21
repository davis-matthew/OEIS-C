/*
 * A206773  Sum of the nonprime proper divisors (or nonprime aliquot parts) of
 *          n.
 * https://oeis.org/A206773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of proper divisors d of n that are not prime
 *            (including 1). a(1)=0. See OEIS A206773
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t) */

/* Compute integer square root of n: largest i such that i*i <= n */
static idx_t isqrt_u64(idx_t n)
{
    idx_t i = 1;
    while (i * i <= n) {
        ++i;
    }
    return i - 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, no proper divisors */

    seq_t sum = 0;
    idx_t sqrt_n = isqrt_u64(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;

            /* d is always < n for n > 1 */
            if (!is_prime(d))
                sum += (seq_t)d;

            if (other != d && other < n && !is_prime(other))
                sum += (seq_t)other;
        }
    }
    return sum;
}
