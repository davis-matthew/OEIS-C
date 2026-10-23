/*
 * A045321  Primes congruent to {1, 2, 3} (mod 5).
 * https://oeis.org/A045321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 1, 2, 3 (mod 5)
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper to expand a dynamic array of the same type as *arr. */
static void grow_array(void **arr, size_t *cap, size_t new_len, size_t elem_size)
{
    if (new_len <= *cap)
        return;
    size_t new_cap = *cap ? *cap : 4;
    while (new_cap < new_len)
        new_cap <<= 1;
    void *t = realloc(*arr, new_cap * elem_size);
    *arr = t;
    *cap = new_cap;
}

/* Test primality of n by trial division using the primes already found. */
static bool is_prime_uint64(idx_t n, idx_t *primes, size_t primes_len)
{
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if ((n & 1ULL) == 0)
        return false;
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if ((idx_t)p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Return the nth prime congruent to 1, 2, or 3 modulo 5 (a(1)=2). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0, but safe guard */

    /* Cache of qualifying primes (return values) */
    static seq_t *qual = NULL;
    static size_t qual_len = 0, qual_cap = 0;

    /* List of all primes found (for trial division) */
    static idx_t *pr = NULL;
    static size_t pr_len = 0, pr_cap = 0;

    /* Current integer to test for primality */
    static idx_t current = 2;

    while (qual_len < (size_t)n) {
        /* Find next prime >= current */
        idx_t candidate = current;
        while (!is_prime_uint64(candidate, pr, pr_len))
            ++candidate;

        /* candidate is prime */
        /* Add to the primes list */
        grow_array((void **)&pr, &pr_cap, pr_len + 1, sizeof(idx_t));
        pr[pr_len++] = candidate;

        /* If it is congruent to 1,2,3 mod 5, store it in the cache */
        if (candidate % 5 != 0 && candidate % 5 != 4) {
            grow_array((void **)&qual, &qual_cap, qual_len + 1, sizeof(seq_t));
            qual[qual_len++] = (seq_t)candidate;
        }

        current = candidate + 1;
    }

    return qual[(size_t)n - 1];
}
