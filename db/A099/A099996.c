/*
 * A099996  a(n) = lcm{1, 2, ..., 2*n}.
 * https://oeis.org/A099996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm{1, 2, ..., 2*n}
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto, gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

/* Cached prime list */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t   prime_limit = 0;

/* Grow the prime cache so that it contains all primes ≤ limit */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Free previous list if any */
    free(prime_list);

    /* Build a new list of primes up to 'limit' */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Compute a(n) = LCM(1,2,…,2*n) */
seq_t
a(idx_t n)
{
    /* Upper bound for the LCM */
    idx_t m = 2 * n;

    /* Ensure we have all primes up to m */
    ensure_primes(m);

    seq_t result = (seq_t)1;

    /* For each prime p <= m, multiply the highest power of p ≤ m */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > m)
            break;

        /* Compute the greatest power of p that does not exceed m */
        uint64_t power = p;
        while (power <= m / p)
            power *= p;

        result = seq_mul(result, (seq_t)power);
    }

    return result;
}
