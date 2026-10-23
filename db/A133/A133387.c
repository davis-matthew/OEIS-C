/*
 * A133387  Greatest prime p such that 6*n-p is prime.
 * https://oeis.org/A133387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the greatest prime p ≤ 6*n such that (6*n – p) is prime.
 *            The function uses a static prime table that is rebuilt for the
 *            largest limit seen so far, allowing calls with arbitrary n order
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/build_primes.h"
#include "../utils/is_prime_big.h"

static idx_t *prime_list   = NULL;   /* array of primes up to prime_limit */
static size_t prime_count  = 0;      /* number of primes in prime_list   */
static idx_t prime_limit   = 0;      /* current maximum limit covered    */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* limit for the sieve: largest prime that could appear */
    idx_t limit = (idx_t)6 * n;

    /* Rebuild the prime table if necessary */
    if (limit > prime_limit) {
        free(prime_list);
        build_primes(limit, &prime_list, &prime_count);
        prime_limit = limit;
    }

    /* Binary search to find the largest prime ≤ limit */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0) return 0;       /* should never happen (2 is prime) */
    size_t top = lo - 1;         /* index of largest prime ≤ limit */

    /* Search downward for the required pair (p, 6*n‑p) */
    size_t i = top;
    while (1) {
        idx_t p = prime_list[i];
        seq_t diff = (seq_t)6 * (seq_t)n - (seq_t)p;
        if (diff >= (seq_t)2 && is_prime_big(diff))
            return (seq_t)p;
        if (i == 0) break;
        --i;
    }

    return 0;   /* should never be reached */
}
