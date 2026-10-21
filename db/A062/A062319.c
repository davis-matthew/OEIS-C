/*
 * A062319  Number of divisors of n^n, or of A000312(n).
 * https://oeis.org/A062319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{p^e‖n} (1 + n·e) (OEIS A062319)
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 329, u64 n <= 6929,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 (0^0 is defined as 1 here) and n = 1.
       For n = 0 there are no prime factors, so product is empty → 1. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Factor n by trial division.  n ≤ 10 000 in practice,
       so this is fast enough. */
    uint64_t temp = (uint64_t)n;
    seq_t prod = (seq_t)1;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* Count exponent e of prime p. */
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* Multiply by (n·e + 1).  Cast to seq_t before multiplying. */
            prod *= (seq_t)((seq_t)n * (seq_t)e + (seq_t)1);
        }
    }

    /* Remaining prime factor (if any). */
    if (temp > 1) {
        prod *= (seq_t)((seq_t)n * (seq_t)1 + (seq_t)1);
    }

    return prod;
}
