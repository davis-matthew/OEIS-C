/*
 * A074040  Product of first n twin prime pair products.
 * https://oeis.org/A074040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A074040 – product of first n twin prime pair products.
   a(1) = 15, a(n) = a(n-1) * (p * (p+2)) where p is the nth starting
   prime of a twin prime pair.  This uses a simple prime trial testing
   via is_prime_small.  Memoisation allows any order of calls. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(uint64_t) */
#include "../utils/seq_realloc.h"      /* seq_realloc(seq_t*, size_t, size_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; not used normally */

    /* Persistent memoisation state */
    static seq_t *terms = NULL;          /* terms[0] = a(1), etc. */
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t next_candidate = 3;      /* next odd to test for primality */
    static seq_t prod = 1;                /* product so far */
    static idx_t twin_cnt = 0;            /* number of twin pairs processed */

    while (twin_cnt < n)
    {
        /* Find next prime */
        while (!is_prime_small((uint64_t)next_candidate))
            next_candidate += 2;          /* skip even numbers */

        idx_t p = next_candidate;
        next_candidate += 2;              /* candidate after p */

        /* Check for twin prime pair (p, p+2) */
        if (is_prime_small((uint64_t)(p + 2)))
        {
            seq_t pair_prod = (seq_t)p * (seq_t)(p + 2);
            prod *= pair_prod;

            /* Grow cache if necessary */
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                terms = tmp;
                terms_cap = new_cap;
            }

            terms[terms_len++] = prod;
            twin_cnt++;
        }
    }

    return terms[n - 1];
}
