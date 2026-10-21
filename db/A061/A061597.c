/*
 * A061597  Numbers whose sum of digits s equals its product of digits and s +
 *          1 and s - 1 are both prime.
 * https://oeis.org/A061597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer k such that digit_sum(k) ==
 *            digit_product(k) and * digit_sum(k)-1 and digit_sum(k)+1 are
 *            prime
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Small prime table up to 200.
 * The digit sum of any number fitting in seq_t is guaranteed to be <= 200
 * (maximum 21 digits of 9 gives 189). */
static bool prime_small[202];
static bool primes_inited = false;

static void init_primes(void)
{
    for (int i = 2; i <= 200; ++i) prime_small[i] = true;
    for (int i = 2; i * i <= 200; ++i)
        if (prime_small[i])
            for (int j = i * i; j <= 200; j += i)
                prime_small[j] = false;
    primes_inited = true;
}

/* Check whether a number satisfies the required properties.
   It returns true iff
     * sum of its decimal digits == product of its decimal digits
     * sum-1 and sum+1 are prime numbers
   Zero digits automatically fail because their product would be 0. */
static bool good(uint64_t v)
{
    unsigned sum = 0;
    uint64_t prod = 1;
    uint64_t x = v;

    if (x == 0) return false;

    while (x > 0) {
        unsigned d = x % 10;
        if (d == 0) return false;      /* product would become 0 */
        sum += d;
        prod *= d;
        x /= 10;
    }

    if (sum != prod) return false;
    if (!prime_small[sum - 1] || !prime_small[sum + 1]) return false;
    return true;
}

/* Return the nth term (1‑based).  The sequence is computed on demand and
   memoised in a static array.  Subsequent calls may come in any order
   and will reuse previously found terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static idx_t last_checked = 1;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (!primes_inited) init_primes();

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Search for the next valid number. */
        while (!good(last_checked))
            ++last_checked;

        /* Store it. */
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)last_checked;
        ++last_checked;
    }

    return cache[n - 1];
}
