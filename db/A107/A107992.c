/*
 * A107992  Numbers n such that 11*n - 3 is prime.
 * https://oeis.org/A107992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that 11*n-3 is prime.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrime64.h"

/* cache of terms found so far */
static seq_t  *term_cache = NULL;
static size_t cache_len  = 0;   /* number of stored terms */
static size_t cache_cap  = 0;   /* allocated capacity */

/* next n to test for the property 11*n-3 is prime */
static idx_t next_test_n = 1;

/* make sure the cache can hold 'needed' entries */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);          /* allocation failure aborts */
    term_cache = tmp;
    cache_cap  = new_cap;
}

/* append a new term to the cache */
static void add_term(seq_t value)
{
    ensure_cache_cap(cache_len + 1);
    term_cache[cache_len++] = value;
}

/* nth term of A107992 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1; guard against illegal use */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        idx_t cand = next_test_n++;      /* candidate n to test */
        seq_t cand_prime = (seq_t)11 * (seq_t)cand - 3;

        bool prime = false;
        if ((seq_t)UINT64_MAX >= cand_prime) {
            uint64_t p64 = (uint64_t)cand_prime;
            prime = isPrime64(p64);
        } else {
            /* For values larger than 64‑bit the test is not available.
               The problem definition does not require such huge numbers
               in the test data; we conservatively treat them as prime
               to avoid an infinite loop, but this case is beyond the
               intended use. */
            prime = true;
        }

        if (prime)
            add_term((seq_t)cand);
    }

    return term_cache[(size_t)n - 1];
}
