/*
 * A069804  Larger of the two consecutive palindromic primes.
 * https://oeis.org/A069804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_palindrome, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A069804 – larger of two consecutive palindromic primes
 * Computed by incremental prime search; for each new prime p the
 * previous prime prev is checked.  If both p and prev are decimal
 * palindromes, p is appended to the cache.  The function returns the
 * n-th cached value. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_palindrome.h"

/* Static cache for recursive calls */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;

/* State for incremental prime generation */
static idx_t prime_scan_start = 1; /* starting point for next_prime() */
static seq_t last_prime = 0;      /* last prime found */

/* Ensure the cache can hold at least `need` elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Grow cache until we have at least n terms */
    while (seq_cache_len < n)
    {
        /* Find next prime > prime_scan_start */
        idx_t next = next_prime(prime_scan_start);
        seq_t p = next; /* implicit conversion from uint64_t */

        /* Check consecutive palindromic pair */
        if (last_prime)
        {
            if (is_palindrome(p, 10) && is_palindrome(last_prime, 10))
            {
                /* p is the larger of the pair */
                ensure_cache_capacity(seq_cache_len + 1);
                seq_cache[seq_cache_len++] = p;
            }
        }

        /* Prepare for next iteration */
        last_prime = p;
        prime_scan_start = next; /* next_prime will search > this value */
    }

    /* n is 1‑based; cache is 0‑based */
    return seq_cache[n - 1];
}
