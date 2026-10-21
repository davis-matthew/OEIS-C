/*
 * A072195  Replace all prime factors p of n with n/p.
 * https://oeis.org/A072195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(A001222(n)-1)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where A001222(n) is the total number of prime factors counted with multiplicity */

#include <stdint.h>

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* count prime factors of n counting multiplicity */
    uint64_t temp = (uint64_t)n;
    int omega = 0;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        while (temp % p == 0) {
            ++omega;
            temp /= p;
        }
    }
    if (temp > 1) {
        ++omega;
    }

    /* exponent is Omega(n) - 1 */
    idx_t exp = (idx_t)(omega - 1);

    /* compute n^exp using binary exponentiation */
    seq_t base = (seq_t)n;
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= (idx_t)1;
    }

    return result;
}
