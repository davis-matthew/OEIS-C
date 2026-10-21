/*
 * A083684  Numbers k such that there is no nonnegative integer m such that m
 *          < k*prime(k) and the concatenated decimal number fp(k,m) =
 *          prime(1).m.prime(2).m. ... .prime(k-1).m.prime(k) is prime.
 * https://oeis.org/A083684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th integer k such that there is no non‑negative
   m < k*prime(k) with fp(k,m) prime; equivalently k == 3 or
   (k % 3 == 1 and (sum_{i=1..k} prime(i)) mod 3 == 0). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

/* Cache for the sequence values, grown lazily. */
static seq_t *cache    = NULL;   /* array of computed a(i) */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of values stored */

/* State for the incremental generation of candidates.  scan_k is the
   next k to examine (starting with 1).  sum_mod3 holds the running sum
   of primes modulo 3. */
static idx_t  scan_k      = 1;
static unsigned int sum_mod3 = 0;

/* Append a new term to the cache.  The value is stored as seq_t. */
static void append_term(idx_t k)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)k;
}

/* Return the n-th term (n>=1).  For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t k = scan_k++;                    /* next candidate k   */
        uint32_t pk = get_prime(k);            /* k‑th prime         */
        sum_mod3 = (sum_mod3 + (pk % 3)) % 3;   /* update running sum */

        /* Condition characterising the sequence. */
        if (k == 3 || ((k % 3 == 1) && (sum_mod3 == 0))) {
            append_term(k);
        }
    }

    return cache[(size_t)n - 1];
}
