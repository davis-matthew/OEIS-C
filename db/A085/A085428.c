/*
 * A085428  Sum of the smallest and largest prime divisors of the n-th
 *          composite number.
 * https://oeis.org/A085428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of smallest and largest prime divisors of the n-th
 *            composite number
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   ensure_capacity, is_prime, largest_prime_divisor,
 *            smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* utils */
#include "../utils/is_prime.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/largest_prime_divisor.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* number of values cached */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t next_num = 4;           /* next integer to test for compositeness */

/* Helper: extend cache up to and including index n */
static void
extend_cache(idx_t n)
{
    /* current candidate number to test */
    idx_t m = next_num;

    while (cache_len <= (size_t)n) {
        /* test composite */
        if (!is_prime(m)) {
            seq_t val = (seq_t)smallest_prime_factor((uint64_t)m)
                        + (seq_t)largest_prime_divisor((uint64_t)m);

            /* grow cache if needed */
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = val;
        }
        ++m;
    }
    next_num = m;  /* next candidate for future calls */
}

seq_t a(idx_t n)
{
    /* ensure cache contains value at index n */
    if (cache_len <= (size_t)n)
        extend_cache(n);
    return cache[(size_t)n];
}
