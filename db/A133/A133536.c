/*
 * A133536  Sum of fifth powers of two consecutive primes.
 * https://oeis.org/A133536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 20, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   build_primes, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A050997(n) + A050997(n+1)
 *   where A050997(k) = p_k^5 and p_k is the k-th prime.
 *
 * The function computes the sum of fifth powers of the two consecutive primes
 * indexed by n.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_primes.h"
#include "../utils/pow_int.h"

/* static storage for primes */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t last_limit = 0;

/* ensure that prime_list contains at least (n+1) primes (p_n and p_{n+1}) */
static void
ensure_primes(idx_t n)
{
    const size_t required = (size_t)n + 1;
    if (prime_count >= required)
        return; /* already have enough */

    /* start with a reasonable limit; double until sufficient */
    idx_t limit = last_limit ? last_limit : 200000;

    while (prime_count < required) {
        if (prime_list)
            free(prime_list);                 /* discard old array */

        build_primes(limit, &prime_list, &prime_count);
        last_limit = limit;

        if (prime_count < required)
            limit *= 2;                       /* increase limit */
    }
}

/* compute x^5 for a seq_t value */
static seq_t
five_power(seq_t x)
{
    return pow_int(x, 5);
}

/* main sequence function */
seq_t
a(idx_t n)
{
    if (n < 1)            /* undefined for n < 1, but return 0 for safety */
        return 0;

    ensure_primes(n);

    seq_t p1 = (seq_t)prime_list[(size_t)n - 1];
    seq_t p2 = (seq_t)prime_list[(size_t)n];

    return five_power(p1) + five_power(p2);
}
