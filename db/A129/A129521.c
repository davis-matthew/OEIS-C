/*
 * A129521  Numbers of the form p*q, p and q prime with q=2*p-1.
 * https://oeis.org/A129521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005382(n) * A005383(n) (the product of the n‑th prime p
 *            and the prime q=2p–1)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 465, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *terms = NULL;    /* cached sequence values */
static size_t terms_len = 0;   /* number of values stored */
static size_t terms_cap = 0;   /* capacity allocated for terms */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* the sequence is 1‑offset */

    /* Ensure we have at least n terms cached. */
    static idx_t next_candidate = 2; /* next prime to test (p) */

    while (terms_len < n) {
        /* Find the next qualifying pair (p, q) with q = 2p – 1 prime. */
        while (true) {
            idx_t p = next_candidate;

            /* Advance to the next candidate prime. */
            if (p <= 2) {
                next_candidate = 3;     /* after 2, only odd numbers */
            } else {
                next_candidate += 2;
            }

            if (!is_prime_big((seq_t)p))
                continue;                 /* p must be prime */

            seq_t q = (seq_t)p * 2u - 1u; /* q = 2p – 1 */
            if (!is_prime_big(q))
                continue;                 /* q must be prime */

            seq_t prod = (seq_t)p * q;    /* product fits in seq_t */

            /* Grow the cache if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();              /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }

            terms[terms_len++] = prod;
            break;                         /* ready for next n */
        }
    }

    return terms[n - 1];
}
