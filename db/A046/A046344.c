/*
 * A046344  Sum of the prime factors of the odd composite numbers (counted
 *          with multiplicity).
 * https://oeis.org/A046344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the prime factors (with multiplicity) of the nth
 *            odd composite number. The sequence is computed lazily and
 *            memoised, so that each call to a(n) runs in near‑linear time up
 *            to the requested index
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: primality test for odd numbers.  Works for arbitrary‑precision seq_t. */
static bool is_prime(seq_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (seq_t p = 3; p * p <= x; p += 2)
        if (x % p == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* 0‑based index for internal storage */
    if (n == 0) return 0;   /* undefined for n < 1 – behaviour outside offset */

    static seq_t *cache = NULL;            /* memoised sequence values */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static seq_t next_odd = 9;             /* next odd composite candidate */

    /* Grow cache if necessary (doubling strategy) */
    if (cache_len < (size_t)n) {
        size_t new_cap = (cache_len == 0) ? 16 : cache_len * 2;
        while (new_cap < n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* allocation error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have computed a(n) */
    while (cache_len < (size_t)n) {
        seq_t candidate = next_odd;

        /* Skip primes */
        while (is_prime(candidate))
            candidate += 2;

        /* Factor candidate and sum the prime factors (with multiplicity) */
        seq_t sum = 0;
        seq_t x = candidate;

        for (seq_t p = 3; p * p <= x; p += 2) {
            while (x % p == 0) {
                sum += p;
                x /= p;
            }
        }
        if (x > 1) sum += x;   /* remaining prime factor */

        cache[cache_len] = sum;
        cache_len++;

        next_odd = candidate + 2;   /* next odd candidate after this composite */
    }

    return cache[n - 1];
}
