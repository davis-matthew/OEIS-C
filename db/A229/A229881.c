/*
 * A229881  Palindromic prime numbers == 8 (mod 9).
 * https://oeis.org/A229881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 966, u128 n <= 966,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic primes that are congruent to 8 (mod 9).
   The function generates odd‑length palindromes in increasing order,
   checks the congruence and primality, caches the results, and returns
   the n‑th term (1‑based).  The cache makes repeated or out‑of‑order
   calls efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

/* Return the n‑th element of A229881.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of stored terms   */
    static size_t cap = 0;      /* allocated capacity       */
    static uint64_t nextHalf = 1;   /* next half to generate
                                      palindrome from (half)   */

    /* generate terms until we have at least n of them */
    while (cached < (size_t)n) {
        /* build odd‑length palindrome from nextHalf */
        uint64_t half = nextHalf;
        seq_t pal = half;                /* start with the half part */
        uint64_t p = half / 10;          /* drop the middle digit for
                                            odd length palindrome */
        while (p > 0) {
            pal = pal * 10 + (seq_t)(p % 10);
            p /= 10;
        }

        /* test congruence mod 9 */
        if ((pal % 9) == 8) {
            /* test primality */
            if (is_prime_big(pal)) {
                /* store term in cache */
                ensure_capacity(cached + 1, &cap, (void **)&cache,
                                sizeof(seq_t));
                cache[cached++] = pal;
            }
        }
        ++nextHalf;
    }

    return cache[(size_t)n - 1];
}
