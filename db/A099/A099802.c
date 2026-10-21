/*
 * A099802  Bisection of A000720.
 * https://oeis.org/A099802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000720(2*n) = number of primes ≤ 2 × n
 * Range:     u8 n <= 809, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime storage – dynamically expanded as higher limits are needed. */
static uint64_t *prime_table = NULL;   /* ascending list of primes */
static size_t   prime_count = 0;       /* number of primes stored */
static size_t   prime_cap   = 0;       /* allocated capacity of prime_table */
static uint64_t max_checked = 1;       /* largest number already tested */

/* Extend the prime table to contain all primes ≤ limit. */
static void primes_up_to(uint64_t limit)
{
    if (limit <= max_checked)
        return;

    for (uint64_t cand = max_checked + 1; cand <= limit; ++cand)
    {
        if (cand < 2)
            continue;

        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint64_t p = prime_table[i];
            /* Stop if p² > cand to avoid unnecessary checks. */
            if ((__int128)p * p > cand)
                break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            if (prime_count == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(prime_table, new_cap * sizeof(uint64_t));
                if (!tmp)
                    exit(1); /* Allocation failure – terminate. */
                prime_table = tmp;
                prime_cap = new_cap;
            }
            prime_table[prime_count++] = cand;
        }
    }
    max_checked = limit;
}

/* Binary search – number of stored primes ≤ limit. */
static size_t count_primes_le(uint64_t limit)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_table[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo; /* count of primes ≤ limit */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behaviour below offset is irrelevant */

    uint64_t limit = (uint64_t)n * 2ULL; /* compute 2 × n safely */
    primes_up_to(limit);
    return (seq_t)count_primes_le(limit);
}
