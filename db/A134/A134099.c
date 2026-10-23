/*
 * A134099  Odd nonprimes np such that np-2 is a prime number but np+2 is not.
 * https://oeis.org/A134099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th odd non‑prime np with np-2 prime and np+2 not
 *            prime
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache of already discovered terms. */
static seq_t *terms = NULL;   /* array of found terms                 */
static size_t terms_size = 0; /* number of terms stored                */
static size_t terms_cap   = 0; /* allocated capacity                    */
static uint64_t next_candidate = 3; /* next odd number to test           */

/* Ensure the cache can accommodate `need` terms. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;

    seq_t *newptr;
    if (terms_cap == 0)
        newptr = seq_alloc(new_cap);
    else
        newptr = seq_realloc(terms, terms_cap, new_cap);

    terms = newptr;
    terms_cap = new_cap;
}

/* Return the n-th term of A134099 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, harmless */

    /* Extend cache until it has at least n elements. */
    while (terms_size < (size_t)n) {
        /* Find next candidate satisfying the condition. */
        for (;;) {
            if (!is_prime_big((seq_t)next_candidate) &&
                is_prime_big((seq_t)(next_candidate - 2)) &&
                !is_prime_big((seq_t)(next_candidate + 2))) {
                break; /* candidate found */
            }
            next_candidate += 2; /* try next odd number */
        }

        /* Append candidate to cache. */
        grow_terms(terms_size + 1);
        terms[terms_size++] = (seq_t)next_candidate;

        /* Advance to odd number after the found candidate. */
        next_candidate += 2;
    }

    return terms[(size_t)n - 1];
}
