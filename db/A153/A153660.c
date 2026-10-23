/*
 * A153660  Pairs of terms of A038512 which differ by 2.
 * https://oeis.org/A153660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2047, u32 n <= 2514, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A153660 – pairs of consecutive terms of A038512 that differ by 2
   Algorithm: iterate over odd numbers, test membership in A038512
   (composite and all prime factors ≥ 13) using is_prime_big.
   When two consecutive such numbers differ by 2, store them in a static
   cache and return the n‑th term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static inline bool in_A038512(seq_t x)
{
    /* A038512 numbers are odd composites with prime factors ≥ 13. */
    if (x <= 1u)
        return false;
    /* Reject if divisible by any prime < 13. */
    if (x % 3u == 0u) return false;
    if (x % 5u == 0u) return false;
    if (x % 7u == 0u) return false;
    if (x % 11u == 0u) return false;
    /* Reject primes ≥ 13. */
    if (is_prime_big(x))
        return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_candidate = 13; /* first odd number ≥ 13 */

    while (terms_len < n)
    {
        seq_t cand = next_candidate;
        next_candidate += 2;  /* next odd number */

        if (!in_A038512(cand))
            continue;

        seq_t cand_plus2 = cand + 2u;
        if (!in_A038512(cand_plus2))
            continue;

        /* Ensure capacity for two more terms. */
        if (terms_len + 2 > terms_cap)
        {
            size_t new_cap = terms_cap ? terms_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1); /* allocation failure */
            terms = tmp;
            terms_cap = new_cap;
        }

        terms[terms_len++] = cand;
        terms[terms_len++] = cand_plus2;
    }

    return terms[n - 1];
}
