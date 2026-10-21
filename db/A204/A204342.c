/*
 * A204342  a(n) = (-1)^n * Sum_{2*m + 1 | 2*n + 1} (-1)^m (2*m + 1)^4.
 * https://oeis.org/A204342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n * Σ_{2*m+1 | 2*n+1} (-1)^m (2*m+1)^4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for the OEIS sequence A204342.  The function handles
 * arbitrary-sized indices (idx_t), but relies on trial division to
 * enumerate the odd divisors of 2*n+1.  The result is returned as
 * seq_t and is guaranteed to be nonnegative for all n. */
seq_t a(idx_t n)
{
    /* 2*n + 1 as a 128‑bit integer to avoid overflow for large n. */
    __int128 odd = (__int128)n << 1;
    odd += 1;

    __int128 sum = 0;        /* signed accumulator for the divisor sum */

    /* Enumerate odd divisors d of odd.  d goes from 1 to sqrt(odd). */
    uint64_t d = 1;
    for (; (__int128)d * d <= odd; d += 2) {
        if (odd % d == 0) {
            /* process divisor d itself */
            unsigned int mpar = (unsigned int)((d >> 1) & 1); /* parity of m = (d-1)/2 */
            __int128 d2 = (__int128)d * d;
            __int128 d4 = d2 * d2;
            sum += (mpar ? -d4 : d4);

            /* process the complementary divisor odd/d if distinct */
            uint64_t q = (uint64_t)(odd / d);
            if (q != d) {
                unsigned int mpar_q = (unsigned int)((q >> 1) & 1);
                __int128 q2 = (__int128)q * q;
                __int128 q4 = q2 * q2;
                sum += (mpar_q ? -q4 : q4);
            }
        }
    }

    /* Multiply by (-1)^n */
    if (n & 1)
        sum = -sum;

    /* sum is now nonnegative; cast to seq_t (unsigned) safely. */
    return (seq_t)sum;
}
