/*
 * A141099  Number of unordered pairs of odd nonprime numbers that sum to 2n.
 * https://oeis.org/A141099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of unordered pairs of odd nonprime numbers that
 *            sum to 2*n
 * Range:     u8 n <= 929, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Static cache of prime status up to current maximum limit */
static bool *is_prime_cache = NULL;   /* NULL indicates uninitialized */
static idx_t sieve_limit = 0;

/* Ensure sieve of Eratosthenes is computed up to the requested limit */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate new array (old array will be freed) */
    bool *new_cache = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_cache)
        return;  /* silently fail; caller will not get valid data, but this
                    should never happen for requested sizes. */
    for (idx_t i = 0; i <= limit; ++i)
        new_cache[i] = true;
    new_cache[0] = new_cache[1] = false;

    /* Classic sieve */
    for (idx_t p = 2; p * p <= limit; ++p)
    {
        if (new_cache[p])
        {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                new_cache[mult] = false;
        }
    }

    /* Replace old cache */
    if (is_prime_cache)
        free(is_prime_cache);
    is_prime_cache = new_cache;
    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t limit = 2 * n;
    ensure_sieve(limit);

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        if ((k & 1U) && !is_prime_cache[k])   /* k is odd nonprime */
        {
            idx_t j = limit - k;              /* j = 2*n - k, always odd */
            if (j <= limit && !is_prime_cache[j]) /* j is odd nonprime */
                count++;
        }
    }
    return count;
}
