/*
 * A034676  Sum of squares of unitary divisors of n.
 * https://oeis.org/A034676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} (p^(2·e) + 1)
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Boyer-Lindquist multiplicative formula for the sum of squares of
   unitary divisors. */

#include <stdint.h>
#include <stdbool.h>

static seq_t pow_small(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        result *= base;
        exp--;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* defined only for n ≥ 1, but guard against 0 */

    seq_t result = 1;
    uint64_t t = (uint64_t)n;

    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint64_t exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            seq_t term = pow_small((seq_t)p, 2 * exp);
            term += 1;
            result *= term;
        }
    }

    /* If t > 1, it is a prime factor with exponent 1 */
    if (t > 1) {
        seq_t term = pow_small((seq_t)t, 2 * 1);
        term += 1;
        result *= term;
    }

    return result;
}
