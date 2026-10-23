/*
 * A156878  Number of primes <= n that are neither safe primes and nor Sophie
 *          Germain primes.
 * https://oeis.org/A156878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2410, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A156878 – Number of primes ≤ n that are neither safe primes
   nor Sophie Germain primes.  a(n) = |{ p ≤ n : p prime,
              (p-1)/2 not prime, 2p+1 not prime }|   */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

static idx_t max_n = 0;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Ensure we have a prime list up to at least 'limit'. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= max_n)
        return;
    if (primes != NULL)
        free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    max_n = limit;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    ensure_primes(n);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n)
            break;
        bool safe = is_prime((idx_t)((p - 1) / 2));
        bool sg   = is_prime((idx_t)(2 * p + 1));
        if (!safe && !sg)
            ++count;
    }
    return count;
}
