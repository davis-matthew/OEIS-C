/*
 * A079088  Number of divisors of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(((prime(n)+1)*(prime(n+1)+1))/4)
 * Range:     u8 n <= 571, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ(x) is the number of positive divisors of x. */
/* Uses the provided utilities get_prime(idx_t) and divisor_count_u64(uint64_t). */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n < 1) return 0;          /* undefined for n = 0, return 0 for safety */

    /* Fetch the n-th and (n+1)-th primes.  get_prime returns uint32_t,
       which is safely cast to uint64_t for the following arithmetic. */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t q = (uint64_t)get_prime(n + 1);

    /* Compute ((p+1)*(q+1))/4.  The product fits into 64 bits for all
       n in the required range (≤ 10000). */
    uint64_t numerator = (p + 1) * (q + 1);
    uint64_t val = numerator / 4;   /* guaranteed to be integer */

    /* Count divisors using the supplied utility.  The result fits into
       seq_t because τ(val) is at most a few thousand for these sizes. */
    uint64_t d = divisor_count_u64(val);
    return (seq_t)d;
}
