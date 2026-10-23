/*
 * A173579  Natural numbers n which give primes when 1331 = 11^3 is prefixed.
 * https://oeis.org/A173579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173579: a(n) = the n‑th natural number m (m ≥ 1) for which the decimal concatenation 1331‖m is prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    /* Cache of found terms.  The cache is static so that calls may arrive in any order. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_size = 0;
    static idx_t tested = 0;              /* Largest candidate examined so far */

    /* Generate terms until we have at least n of them. */
    while (terms_size < n)
    {
        idx_t cand = tested + 1;
        tested = cand;

        /* If cand ≡ 1 (mod 3) the concatenated number is a multiple of 3, skip it. */
        if (cand % 3 == 1) continue;

        /* Concatenate 1331 and cand. */
        seq_t val = concat_parts(1331, cand);

        /* If the concatenated number is not prime, skip this candidate. */
        if (!is_prime_big(val)) continue;

        /* Grow the cache if necessary. */
        if (terms_size + 1 > terms_cap)
        {
            size_t new_cap = terms_cap ? terms_cap * 2 : 8;
            seq_t *tmpv = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmpv) abort();    /* allocation failed */
            terms = tmpv;
            terms_cap = new_cap;
        }

        terms[terms_size++] = (seq_t)cand;
    }

    return terms[n - 1];
}
