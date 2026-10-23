/*
 * A125239  Smallest prime divisor of 10*T(n)+1 = 5*n*(n+1)+1, where T(n) = 1
 *          + 2 + ... + n.
 * https://oeis.org/A125239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime divisor of 5*n*(n+1) + 1
 * Range:     u8 n <= 6, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Here n is 1‑based, as in the OEIS sequence A125239.) */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) { /* undefined for n < 1, but return 0 to be safe */
        return 0;
    }

    /* Compute M = 5*n*(n+1) + 1 using seq_t arithmetic. */
    seq_t m = (seq_t)5 * n * (n + 1) + 1;

    /* Convert to uint64_t for factorisation – safe for all n up to about 10^5. */
    uint64_t m64 = (uint64_t)m;

    /* All values are odd, but we check 2 just for completeness. */
    if (m64 % 2 == 0) {
        return (seq_t)2;
    }

    /* Trial division by odd numbers up to sqrt(m). */
    uint64_t d = 3;
    uint64_t sqrt_m = (uint64_t) sqrt((long double) m64);
    while (d <= sqrt_m) {
        if (m64 % d == 0) {
            return (seq_t)d;   /* smallest prime factor found */
        }
        d += 2;
    }

    /* m itself is prime. */
    return (seq_t)m64;
}
