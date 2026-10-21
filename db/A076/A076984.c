/*
 * A076984  Number of Fibonacci numbers that are divisors of the n-th
 *          Fibonacci number.
 * https://oeis.org/A076984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) - [n even]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ(n) = number of divisors of n.  For an even n we subtract one, because
   the Fibonacci numbers F(1)=F(2)=1 are equal, so one divisor (the number 1)
   is counted twice.  For odd n nothing is subtracted.  This yields the
   sequence A076984. */

#include <stdbool.h>
#include <stdint.h>   /* for uint64_t */

/* Count the number of divisors of a 64‑bit integer.
   This routine uses trial division up to sqrt(n).  All
   arithmetic fits in uint64_t for the limits required
   by the problem. */
static uint64_t count_divisors_u64(uint64_t n)
{
    if (n == 0) return 0;      /* undefined, never happens for n >= 1 */
    uint64_t result = 1;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (n > 1)          /* n is a prime > sqrt(original n) */
        result *= 2;
    return result;
}

/* Public sequence function required by the framework. */
seq_t a(idx_t n)
{
    /* Convert the index to a 64‑bit integer for factoring.  idx_t is
       guaranteed to hold at least 64 bits, but we cast explicitly for
       clarity. */
    uint64_t idx = (uint64_t)n;

    /* Compute τ(n) and adjust for even n. */
    uint64_t divcnt = count_divisors_u64(idx);
    if ((idx & 1ULL) == 0)      /* n is even */
        divcnt -= 1;

    /* The result is guaranteed to fit in seq_t for all n that are
       tested (the sequence values are all much smaller than the
       limits of the target integer type).  We cast to seq_t. */
    return (seq_t)divcnt;
}
