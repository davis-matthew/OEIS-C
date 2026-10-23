/*
 * A166649  Totally multiplicative sequence with a(p) = 9*(p+1) for prime p.
 * https://oeis.org/A166649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (9*(p+1))^e (multiplicative, a(p)=9*(p+1))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the A166649 sequence value for index n (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* safety for out-of-range */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* trial division factorization – fast enough for n up to 10^4 */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p != 0) continue;
        uint64_t exp = 0;
        while (m % p == 0)
        {
            m /= p;
            ++exp;
        }
        seq_t factor = (seq_t)(9 * (p + 1));
        for (uint64_t i = 0; i < exp; ++i)
            result *= factor;
    }

    /* Remaining prime factor (if any) */
    if (m > 1)
    {
        seq_t factor = (seq_t)(9 * (m + 1));
        result *= factor;   /* exponent is 1 */
    }

    return result;
}
