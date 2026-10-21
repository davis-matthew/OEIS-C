/*
 * A079085  Number of distinct prime factors of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of
 *            ((prime(n)+1)*(prime(n+1)+1))/4 * (i.e. a(n) =
 *            A001221((prime(n)+1)*(prime(n+1)+1)/4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset not defined for n<1 */
    uint32_t p1 = get_prime(n);          /* prime(n) */
    uint32_t p2 = get_prime(n + 1);      /* prime(n+1) */
    uint64_t prod = ((uint64_t)p1 + 1ULL) *
                    ((uint64_t)p2 + 1ULL);
    prod /= 4ULL;                        /* (prime(n)+1)*(prime(n+1)+1)/4 */
    int cnt = count_distinct_primes(prod);
    return (seq_t)cnt;
}
