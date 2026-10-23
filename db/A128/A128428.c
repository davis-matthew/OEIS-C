/*
 * A128428  Number of distinct prime factors of n^2+1.
 * https://oeis.org/A128428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of distinct prime factors of n^2+1
 *          (OEIS A128428, A001221(n^2+1))
 * Computation: factor n^2+1 by trial division using a prime list.
 *
 * The function uses a static prime sieve that is expanded as
 * needed.  For each n, n^2+1 is computed as a seq_t, the integer
 * square root is taken with floor_sqrt(), and the primes up to
 * that root are guaranteed to be present in the sieve.  The
 * number of distinct primes dividing the value is then counted.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

static uint64_t *prime_list = NULL;      /* array of primes */
static size_t      prime_cnt  = 0;       /* number of primes in the list */
static idx_t       prime_limit = 0;      /* largest value for which the sieve was built */

/* Ensure that prime_list contains all primes <= limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;                         /* already sufficient */
    /* Free old list if present. */
    free(prime_list);
    /* Build new sieve up to the required limit. */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Count distinct prime factors of a uint64_t value. */
static seq_t count_distinct_prime_factors(uint64_t x)
{
    int count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x)
            break;
        if (x % p == 0) {
            /* p divides x, count it once. */
            ++count;
            while (x % p == 0)
                x /= p;
        }
    }
    /* If remainder > 1, it is a prime factor. */
    if (x > 1)
        ++count;
    return (seq_t)count;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute n^2 + 1 using seq_t arithmetic. */
    seq_t n_sq = (seq_t)n;
    n_sq *= n_sq;
    seq_t val = n_sq + 1;

    /* Ensure we have primes up to floor(sqrt(val)). */
    seq_t sqrt_val = floor_sqrt(val);
    idx_t needed_limit = (idx_t)sqrt_val + 1;   /* add 1 to be safe */
    ensure_primes_upto(needed_limit);

    /* Factor val (fits in uint64_t for the intended n range). */
    uint64_t num = (uint64_t)val;   /* cast is safe for values up to 2^64-1 */

    return count_distinct_prime_factors(num);
}
