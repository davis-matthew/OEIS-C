/*
 * A179463  Semiprimes A001358 containing at least one semiprime digit in base
 *          10.
 * https://oeis.org/A179463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   has_semiprime_digit, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179463: Semiprimes containing at least one 4, 6 or 9 digit in base 10
   The function computes the n-th such semiprime by testing integers sequentially
   and caching the found values. It uses trial division to test for semiprimality
   and a simple digit scan to check for the required digits. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t n) */
#include "../utils/has_semiprime_digit.h"

/* true if n is a semiprime (product of exactly two primes, possibly equal) */
static bool is_semiprime(uint64_t n)
{
    if (n < 4)          /* smallest semiprime is 4 = 2*2            */
        return false;

    /* Trial division for the smallest prime factor */
    uint64_t p = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            p = d;
            break;
        }
    }
    if (!p)             /* n is prime */
        return false;

    uint64_t m = n / p; /* other factor */
    /* m must be prime for n to be a semiprime */
    return is_prime_big((seq_t)m);
}

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;   /* dynamic array of found terms        */
    static size_t cache_cap = 0;     /* allocated capacity                  */
    static size_t cache_size = 0;    /* number of terms stored              */
    static uint64_t last_checked = 3;/* last integer examined (n < 4 are skipped) */

    if (n == 0)  /* the OEIS offset starts at 1; the behaviour for n==0 is irrelevant */
        return (seq_t)0;

    /* Grow the cache until it contains at least n entries */
    while (cache_size < (size_t)n) {
        ++last_checked;                 /* examine the next integer */

        if (is_semiprime(last_checked) && has_semiprime_digit(last_checked)) {
            /* Allocate more space if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
                if (!tmp) abort();                    /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }

    /* Return the (n‑1)-th element (array is 0‑based) */
    return (seq_t)cache[n - 1];
}
