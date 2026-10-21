/*
 * A056609  a(n) = rad(n!)/rad(A001142(n)) where rad(n) is the squarefree
 *          kernel of n, A007947(n).
 * https://oeis.org/A056609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n!)/rad(A001142(n)) * a(n) is the unique prime p <= n
 *            missing in the prime factorization * of the nth row of Pascal's
 *            triangle, otherwise 1
 * Range:     u8 n <= 285, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static idx_t prime_limit = 0;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Ensure that all primes up to 'limit' are available. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list != NULL)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;            /* There are no primes <=1 */

    ensure_primes_upto(n);

    seq_t n_plus_one = (seq_t)n + 1;

    /* Scan primes <= n */
    for (size_t i = 0; i < prime_count && prime_list[i] <= n; ++i)
    {
        idx_t p = (idx_t)prime_list[i];
        seq_t prod = 1;
        idx_t temp = n;

        /* Compute product of (digit+1) for n in base p */
        while (temp > 0)
        {
            idx_t digit = temp % p;
            prod *= (seq_t)(digit + 1);
            if (prod > n_plus_one)
                break;               /* No need to continue */
            temp /= p;
        }

        if (prod == n_plus_one)
            return (seq_t)p;          /* p is the missing prime */
    }

    return (seq_t)1;
}
