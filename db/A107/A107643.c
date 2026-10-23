/*
 * A107643  Numbers n such that the string 75n is prime.
 * https://oeis.org/A107643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A107643: Numbers n such that the decimal string 75n is prime.
   The function returns the *n*‑th term of the sequence (1‑based index).
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides static bool is_prime(uint64_t) */

/* Dynamic cache for computed terms. */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_size = 0;
static idx_t next_candidate = 1;         /* next n to test */
static uint64_t pow10 = 1;               /* 10^k for current n's digit length */

/* Ensure the cache has at least 'n' terms stored. */
static void ensure_terms(idx_t n)
{
    /* Grow the array on demand. */
    while (terms_size < n)
    {
        /* Update 10^k when the digit length increases. */
        if (next_candidate == pow10)
            pow10 *= 10ULL;

        /* Build the concatenated number 75n. */
        unsigned __int128 val128 = (unsigned __int128)75 * pow10 + next_candidate;
        uint64_t val = (uint64_t)val128;   /* safe for all values needed here */

        /* Test for primality and store if prime. */
        if (is_prime(val))
        {
            if (terms_size == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_size++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }
}

/* Public function requested by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 as guard */
    ensure_terms(n);
    return terms[n - 1];
}
