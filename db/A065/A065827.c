/*
 * A065827  Sum of squares of divisors of square numbers.
 * https://oeis.org/A065827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = σ₂(n²) = ∏_{p^e∥n} (p^(4e+2)-1)/(p^2-1) (sum of squares
 *            of the divisors of the square of n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 233, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* the sequence starts at n = 1; behaviour for n < 1 is irrelevant */
    if (n < 1) return (seq_t)0;

    uint64_t remaining = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor out powers of 2 */
    if ((remaining & 1) == 0) {
        int e = 0;
        while ((remaining & 1) == 0) {
            remaining >>= 1;
            ++e;
        }
        /* compute term for prime 2 */
        seq_t term = (seq_t)1;    /* start with 1 (k = 0) */
        seq_t p2 = (seq_t)4;      /* 2² */
        seq_t pow = (seq_t)1;     /* p^0 initially */
        for (int k = 1; k <= 2 * e; ++k) {
            pow *= p2;        /* p^(2k) */
            term += pow;
        }
        result *= term;
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p <= remaining / p; p += 2) {
        if (remaining % p != 0) continue;

        int e = 0;
        while (remaining % p == 0) {
            remaining /= p;
            ++e;
        }

        seq_t term = (seq_t)1;
        seq_t p2 = (seq_t)p * (seq_t)p;   /* p² */
        seq_t pow = (seq_t)1;
        for (int k = 1; k <= 2 * e; ++k) {
            pow *= p2;
            term += pow;
        }
        result *= term;
    }

    /* remaining prime factor greater than sqrt(original n) */
    if (remaining > 1) {
        uint64_t p = remaining;   /* e = 1 */
        seq_t term = (seq_t)1;
        seq_t p2 = (seq_t)p * (seq_t)p;   /* p² */
        seq_t pow = (seq_t)1;
        /* e = 1, so 2e = 2 */
        for (int k = 1; k <= 2; ++k) {
            pow *= p2;
            term += pow;
        }
        result *= term;
    }

    return result;
}
