/*
 * A128995  For a number n, let r=n+1, s = the least number greater than r and
 *          coprime to nr; n is here if nr-s, rs-n and sn-r are all prime.
 * https://oeis.org/A128995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 28, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128995.
 * For a number n, let r = n+1 and s be the least integer > r that
 * is coprime to n*r.  
 * n is a term of the sequence if the three numbers
 * n*r - s, r*s - n and s*n - r are all prime.
 *
 * This implementation searches for terms one by one, storing found
 * values in a dynamically growing cache.  It works for any supplied
 * seq_t and idx_t types. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* static cache of computed terms */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
static seq_t next_candidate = 1;   /* next n to test */

/* ------------------------------------------------------------------ */
/* ensure that the cache contains at least "need" terms */
static void ensure_terms(idx_t need)
{
    while (cache_sz < need) {
        seq_t n = next_candidate++;          /* next integer to test   */
        seq_t r = n + (seq_t)1;              /* r = n + 1              */
        seq_t mr = n * r;                    /* n * r                  */

        /* find the least s > r coprime to mr */
        seq_t s = r + (seq_t)1;
        while (gcd(mr, s) != (seq_t)1) ++s;

        /* differences to test for primality */
        seq_t d1 = mr - s;        /* n*r - s */
        seq_t d2 = r * s - n;     /* r*s - n */
        seq_t d3 = s * n - r;     /* s*n - r */

        /* check that all are prime (>1) */
        if (d1 > (seq_t)1 && d2 > (seq_t)1 && d3 > (seq_t)1 &&
            is_prime_big(d1) && is_prime_big(d2) && is_prime_big(d3)) {
            /* grow cache if needed */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = n;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* undefined for index 0 */
        return (seq_t)0;
    ensure_terms(n);
    return cache[n - 1];
}
