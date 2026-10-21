/*
 * A097700  Numbers not of form x^2 + 2y^2.
 * https://oeis.org/A097700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097700: Numbers not of the form x^2 + 2y^2 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static const idx_t primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,
    157,163,167,173,179,181,191,193,197,199
};
static const int PRIME_COUNT = sizeof(primes)/sizeof(primes[0]);

/* Check if a number m can be represented as x^2 + 2y^2 */
static bool is_nonrepresentable(idx_t m)
{
    idx_t n = m;
    for (int i = 0; i < PRIME_COUNT && (idx_t)primes[i]*(idx_t)primes[i] <= n; ++i)
    {
        int p = primes[i];
        if (n % p == 0)
        {
            int exp = 0;
            while (n % p == 0)
            {
                n /= p;
                exp ^= 1; /* keep only parity of exponent */
            }
            if ((p % 8 == 5 || p % 8 == 7) && exp == 1)
                return true;
        }
    }
    /* n now is either 1 or a prime > sqrt(m) */
    if (n > 1 && (n % 8 == 5 || n % 8 == 7))
        return true;
    return false;
}

/* Cache of computed nonrepresentable numbers */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Expand cache until we have at least n elements */
    while ((idx_t)cache_len < n)
    {
        idx_t num = ++last_checked;
        if (is_nonrepresentable(num))
        {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)num;
        }
    }
    return cache[n - 1];
}
