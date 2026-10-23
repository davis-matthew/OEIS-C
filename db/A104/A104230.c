/*
 * A104230  Minimal number of primes needed to sum to n^2.
 * https://oeis.org/A104230
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 if n^2 is even or n^2 – 2 is prime, otherwise a(n) = 3
 *            (minimal number of primes needed to sum to n^2)
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* undefined for the offset */

    /* compute n^2 in seq_t */
    seq_t sq = (seq_t)n * ((seq_t)n + 1) / 2 * 2 - (seq_t)n; /* n^2 = n*(n+1)/2*2 - n */
    if ((sq & 1) == 0)             /* even square */
        return 2;

    uint64_t candidate = (uint64_t)sq - 2; /* s - 2 */
    return is_prime(candidate) ? 2 : 3;
}
