/*
 * A036963  Primes with digits (0,...,7) taken as base 8 and converted to base
 *          10.
 * https://oeis.org/A036963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A036963 – Primes with digits (0,…,7) taken as base 8 and converted to base 10
   The n‑th element of the sequence is the n‑th integer p (in ascending
   order) such that when p is written in base 8 and that octal string is
   interpreted as a decimal integer, the result is prime.

   Implementation uses lazy generation and caching.  The candidate integer
   is taken as a normal u64; from it we recover the base‑8 digits,
   reverse them to obtain the correct decimal value, and test primality
   with the supplied `isPrimeGeneric`.  Results are stored in a static
   dynamic array so that arbitrary permutations of calls are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* Return true if the octal representation of decimal number `n` interpreted
   as a decimal integer is prime.  The conversion reconstructs the digits
   in the correct order (most significant first). */
static bool is_octal_dec_prime(uint64_t n)
{
    /* Base-8 digits are obtained least‑significant first.  The
       maximum number of octal digits for a 64‑bit integer is 43. */
    uint8_t digits[64];
    size_t cnt = 0;
    while (n != 0) {
        digits[cnt++] = n & 7ULL;   /* n % 8 */
        n >>= 3;                    /* n /= 8 */
    }
    if (cnt == 0)          /* the number zero itself – not prime */
        return false;

    seq_t m = 0;
    for (size_t i = cnt; i-- > 0; ) {
        m = m * 10 + digits[i];
    }
    return isPrimeGeneric(m);
}

/* Cached sequence values. */
seq_t a(idx_t n)
{
    /* The cache grows lazily.  We keep track of the number of stored terms,
       the current capacity, and the last integer examined. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++last_checked;                               /* next candidate */
        if (is_octal_dec_prime(last_checked)) {
            if (cache_len == cache_cap) {             /* grow cache */
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
