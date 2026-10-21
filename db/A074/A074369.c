/*
 * A074369  Number of divisors of Sum_{i=1..n} prime(i).
 * https://oeis.org/A074369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of divisors of Sum_{i=1..n} prime(i)
 * Range:     u8 n <= 13, u16 n <= 1649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"
#include "../utils/divisor_count.h"

static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static seq_t *prefix_sum = NULL;   /* prefix_sum[0] = 0, prefix_sum[i] = sum of first i primes */
static bool initialized = false;

/* Initialise prime list and cumulative sums.  The bound 300 000 covers the
   first 25997 primes, more than enough for n <= 10000. */
static void init_primes(void)
{
    idx_t limit = 300000;  /* safety margin */
    sieve_primes(limit, &prime_list, &prime_cnt);

    /* allocate and compute prefix sums */
    prefix_sum = (seq_t*)malloc((prime_cnt + 1) * sizeof(seq_t));
    prefix_sum[0] = 0;
    for (size_t i = 1; i <= prime_cnt; ++i)
    {
        prefix_sum[i] = prefix_sum[i - 1] + (seq_t)prime_list[i - 1];
    }

    initialized = true;
}

/* Return the n-th term of A074369 (n >= 1). */
seq_t a(idx_t n)
{
    if (!initialized)
        init_primes();

    /* Safety: if n exceeds known primes, grow the table (unlikely for typical n). */
    if (n > prime_cnt)
    {
        /* Double the sieve limit until we have enough primes. */
        idx_t new_limit = prime_list ? prime_list[prime_cnt - 1] * 2 : 1000;
        while (true)
        {
            free(prime_list);
            free(prefix_sum);
            sieve_primes(new_limit, &prime_list, &prime_cnt);
            if (prime_cnt >= n)
                break;
            new_limit *= 2;
        }
        /* Recompute prefix sums with the new array */
        free(prefix_sum);
        prefix_sum = (seq_t*)malloc((prime_cnt + 1) * sizeof(seq_t));
        prefix_sum[0] = 0;
        for (size_t i = 1; i <= prime_cnt; ++i)
            prefix_sum[i] = prefix_sum[i - 1] + (seq_t)prime_list[i - 1];
    }

    /* Sum of first n primes */
    seq_t sum = prefix_sum[n];

    /* Count divisors of this sum.  sum fits into uint64_t for n up to 10000. */
    uint64_t divs = divisor_count((uint64_t)sum,
                                  (uint64_t*)prime_list,
                                  (int)prime_cnt);

    return (seq_t)divs;
}
