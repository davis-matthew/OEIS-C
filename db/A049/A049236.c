/*
 * A049236  a(n) is the number of distinct prime factors of prime(n) + 2.
 * https://oeis.org/A049236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct prime factors of prime(n)+2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* The offset is 1, so n >= 1 is required for the sequence. */
    uint32_t p = get_prime(n);          /* prime(n) */
    uint64_t m = (uint64_t)p + 2U;      /* prime(n) + 2 */
    int cnt = count_distinct_primes(m); /* number of distinct prime factors */
    return (seq_t)cnt;
}
