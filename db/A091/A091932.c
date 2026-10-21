/*
 * A091932  Primes that remain prime when their leading digit in binary
 *          representation is replaced by 0.
 * https://oeis.org/A091932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 942, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A091932: primes p such that p - 2^⌊log₂p⌋ is also prime.
   a(n) is the nth such prime (n ≥ 1). This implementation searches successive
   primes, stores those that satisfy the condition, and returns the
   requested entry.  It uses the provided is_prime_big() and bitlen_seq_t()
   utilities and caches results across calls. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1 – return 0 for safety */
    if (n < 1)
        return 0;

    /* Static cache of qualifying primes */
    static seq_t *matches  = NULL;
    static size_t matches_cnt = 0;
    static size_t matches_cap = 0;

    /* Static pointer to the last tested prime
       (initialized to 1 so the first loop starts at 2). */
    static seq_t last_candidate = 1;

    /* Generate matches until we have at least n of them. */
    while (matches_cnt < (size_t)n)
    {
        /* Find the next prime greater than last_candidate. */
        do
        {
            ++last_candidate;
        } while (!is_prime_big(last_candidate));

        /* Compute p - 2^⌊log₂p⌋. */
        unsigned shift = bitlen_seq_t(last_candidate) - 1; /* ⌊log₂p⌋ */
        seq_t pow2 = ((seq_t)1) << shift;
        seq_t q    = last_candidate - pow2;

        /* If the modified number is also prime, store the candidate. */
        if (is_prime_big(q))
        {
            if (matches_cnt >= matches_cap)
            {
                matches_cap = matches_cap ? matches_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(matches,
                                               matches_cap * sizeof(seq_t));
                if (!tmp)                  /* allocation failed */
                    return 0;
                matches = tmp;
            }
            matches[matches_cnt++] = last_candidate;
        }
    }

    return matches[n - 1];   /* 1‑based indexing */
}
