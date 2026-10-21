/*
 * A232881  Twin primes with digital root 5 or 7.
 * https://oeis.org/A232881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the n‑th smallest prime p such that (p,p+2) is a twin
   prime pair and the digital roots of p and p+2 are 5 and 7 in some order.
   The values are generated lazily and cached. */

#include <stdint.h>
#include <stdlib.h>

/* The build supplies this rounding‑probe routine */
#include "../utils/is_prime_big.h"

/* Cached sequence */
static seq_t *terms    = NULL;   /* array of found terms      */
static size_t term_len = 0;      /* number of cached terms    */
static size_t term_cap = 0;      /* allocated capacity        */
static uint64_t next_candidate = 2;   /* last examined odd number */

/* Make sure we have enough capacity in the cache */
static void grow_terms(size_t needed)
{
    if (term_cap >= needed) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* terminate on allocation failure */
    terms = tmp;
    term_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined – guard against underflow */

    /* Continue generating terms until we have at least n */
    while (term_len < (size_t)n) {
        /* Advance to next odd candidate */
        if (next_candidate <= 2) {
            next_candidate = 3;
        } else {
            next_candidate += 2;
        }

        seq_t cand   = (seq_t)next_candidate;
        seq_t cand2  = cand + (seq_t)2;

        /* Primality test */
        if (!is_prime_big(cand))      continue;
        if (!is_prime_big(cand2))    continue;

        /* Digital‑root check via modulo 9 */
        int r  = (int)(next_candidate % 9);
        int r2 = (int)((next_candidate + 2) % 9);
        if (!((r == 5 && r2 == 7) || (r == 7 && r2 == 5)))
            continue;

        /* Store found term */
        grow_terms(term_len + 1);
        terms[term_len++] = cand;
    }

    return terms[n - 1];
}
