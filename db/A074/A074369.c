/*
 * A074369  Number of divisors of Sum_{i=1..n} prime(i).
 * https://oeis.org/A074369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of divisors of Sum_{i=1..n} prime(i)
 * Range:     u8 n <= 13, u16 n <= 1649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9736 (2026-11-03)
 * Helpers:   mem, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/sieve_primes.h"
#include "../utils/mem.h"

/* Smallest divisor count helper */
static inline uint64_t divisor_count(uint64_t x, uint64_t *pr, int pcnt)
{
    uint64_t d = 1;
    for (int i = 0; i < pcnt && (uint64_t)pr[i] * pr[i] <= x; ++i) {
        uint64_t p = pr[i];
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            d *= (e + 1);
        }
    }
    if (x > 1) d *= 2;          /* remaining prime factor */
    return d;
}

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
    prefix_sum = mem_new(seq_t, (prime_cnt + 1));
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
            mem_free(prefix_sum);
            sieve_primes(new_limit, &prime_list, &prime_cnt);
            if (prime_cnt >= n)
                break;
            new_limit *= 2;
        }
        /* Recompute prefix sums with the new array */
        mem_free(prefix_sum);
        prefix_sum = mem_new(seq_t, (prime_cnt + 1));
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
