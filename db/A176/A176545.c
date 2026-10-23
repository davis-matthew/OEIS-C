/*
 * A176545  Numbers k>0 such that 2*k^2+14*k+5 is prime.
 * https://oeis.org/A176545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer k such that 2*k^2+14*k+5 is
 *            prime (OEIS A176545). The sequence is indexed from 1
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of discovered k values */
    static seq_t *cache = NULL;           /* dynamically allocated array */
    static size_t cache_len = 0;          /* number of elements stored */
    static size_t cache_cap = 0;          /* allocated capacity (in elements) */
    static idx_t next_k = 1;              /* next k to test */

    if (n == 0)
        return (seq_t)0;                  /* below offset – undefined */

    /* Extend the cache until it contains at least n values */
    while (cache_len < n) {
        seq_t kseq = (seq_t)next_k;                 /* cast candidate k */

        /* Compute p = 2*k^2 + 14*k + 5 using seq_t arithmetic.  The
           cast of the integer literals to seq_t guarantees the
           multiplication and addition are carried out in the seq_t
           type, so no premature truncation occurs. */
        seq_t val = ((seq_t)2) * kseq * kseq
                  + ((seq_t)14) * kseq
                  + ((seq_t)5);

        /* Test primality of p */
        if (is_prime_big(val)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = kseq;      /* store found k */
        }
        ++next_k;                           /* next integer to test */
    }

    return cache[n - 1];
}
