/*
 * A068067  Number of integers m, 0 < m <= n, such that n divides m(m+1)/2.
 * https://oeis.org/A068067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a power of 2 (n>1) a(n) = 2^(omega(n)-1) - 1 if
 *            n is even but not a power of 2 a(n) = 2^omega(n) if n is odd
 *            omega(n) = number of distinct prime divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Power‑of‑two test (special case) */
    if (n > 1 && (n & (n - 1)) == 0)
        return 0;

    /* Count distinct prime factors */
    int omega = count_distinct_primes(n);

    if ((n & 1) == 0)   /* n is even (but not a power of two) */
        return ((seq_t)1 << (omega - 1)) - 1;
    else                 /* n is odd (including n == 1) */
        return ((seq_t)1 << omega);
}
