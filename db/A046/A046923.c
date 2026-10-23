/*
 * A046923  Number of ways to express 2n+1 as p+2a^2; p prime, a >= 0.
 * https://oeis.org/A046923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 2n+1 as p + 2a^2, where p is
 *            prime and a >= 0
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the small-prime test provided by the utilities. */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* m = 2n + 1 as the target odd number */
    seq_t m = ((seq_t)n << 1u) + 1u;   /* cast to seq_t before shifting */

    seq_t count = 0;

    /* Search over a >= 0.  Stop when 2a^2 > m-2 which leaves p < 2. */
    for (idx_t a = 0; ; ++a) {
        seq_t aa = (seq_t)a * a;          /* a^2 */
        seq_t s  = aa + aa;               /* 2a^2 */

        /* If the remaining part would be less than 2, no more primes possible. */
        if (s > m - 2u) break;

        seq_t p = m - s;                 /* candidate prime */
        if (p < 2u) break;
        if (is_prime_small(p))
            ++count;
    }

    return count;
}
