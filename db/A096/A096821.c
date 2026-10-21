/*
 * A096821  Solutions to Mod[sigma(x),x]=12 of the form p*(p+13)/2 where p is
 *          a prime of form=2^j-13, of which j exponents are listed in
 *          A096818.
 * https://oeis.org/A096821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p+13)/2 where p = 2^j-13 is prime and j is the n‑th
 *            exponent for which 2^j-13 is prime
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* pow2(exp) -> seq_t */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) -> bool */

/* Cache of previously computed terms.  The array is resized as needed. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_size = 0;  /* number of terms stored */
static idx_t next_j = 4;       /* next exponent to test; first j where 2^j-13 > 0 */

seq_t a(idx_t n)
{
    /* Return cached value if already available. */
    if (n >= 1 && (size_t)n <= cache_size) {
        return cache[(size_t)n - 1];
    }

    /* Ensure we have enough capacity for new terms. */
    if (cache_cap <= cache_size) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach the requested index. */
    while (cache_size < (size_t)n) {
        /* Compute p = 2^j - 13.  j starts at 4 where this is >0. */
        seq_t p = pow2(next_j) - (seq_t)13;

        /* Only consider prime p. */
        if (is_prime_big(p)) {
            /* a = p * (p+13) / 2.  (p+13) = 2^j is even. */
            seq_t val = p * ((p + (seq_t)13) / 2);
            cache[cache_size++] = val;
        }

        ++next_j;   /* move to next exponent */
    }

    return cache[(size_t)n - 1];
}
