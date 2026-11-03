/*
 * A185102  a(n) is the recursion depth of repeatedly factoring n and then the
 *          exponents in its prime product factorization, until 1 is reached.
 * https://oeis.org/A185102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=1+max{a(k_i)} where k_i are the exponents
 *            in the prime‑factorisation of n (the recurrence from the OEIS
 *            entry)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9467 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Recursively compute the depth of n. */
static seq_t depth_calc(idx_t n)
{
    if (n == 1)
        return (seq_t)0;   /* base case */

    uint64_t m = (uint64_t)n;
    seq_t max_depth = 0;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t d = depth_calc((idx_t)exp);
            if (d > max_depth)
                max_depth = d;
        }
    }

    /* If a prime factor > sqrt(n) remains, its exponent is 1.
       depth_calc(1) = 0, so it does not affect max_depth. */
    if (m > 1)
        (void)depth_calc((idx_t)1);

    seq_t result = max_depth;
    result += (seq_t)1;  /* add the current level */
    return result;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    return depth_calc(n);
}
