/*
 * A072412  Numbers k such that the LCM of exponents in the prime
 *          factorization of k does not equal the largest exponent.
 * https://oeis.org/A072412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1470, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072412 – Numbers k such that the LCM of the exponents
 * in the prime factorization of k does not equal the largest exponent.
 * The nth term (n ≥ 1) is returned.
 */

/* Include helper functions from the utils library. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/gcd.h"          /* gcd */

seq_t a(idx_t n)
{
    /* Static cache of sequence terms. */
    static seq_t *seq_terms = NULL;
    static size_t seq_len  = 0;   /* number of terms cached */
    static size_t seq_cap  = 0;   /* capacity of seq_terms */
    static uint64_t next_k  = 2;  /* next integer to test */

    if (n == 0) return 0; /* sequence defined for n ≥ 1 only */

    /* Generate terms until we have at least n terms cached. */
    while (seq_len < (size_t)n)
    {
        uint64_t k = next_k++;

        /* Factor k into primes and exponents. */
        uint64_t primes[32];
        uint64_t exps[32];
        size_t cnt = 0;
        factor_u64(k, primes, exps, &cnt);

        /* Skip prime powers or 1 (cnt ≤ 1). */
        if (cnt <= 1) continue;

        /* Compute the maximum exponent and the LCM of all exponents. */
        seq_t maxe   = 0;
        seq_t lcm    = 1;
        for (size_t i = 0; i < cnt; ++i)
        {
            seq_t e = (seq_t)exps[i];
            if (e > maxe) maxe = e;
            seq_t g = gcd(lcm, e);
            lcm = (lcm / g) * e;
        }

        /* If LCM ≠ largest exponent, k belongs to the sequence. */
        if (lcm != maxe)
        {
            /* Grow cache if necessary. */
            if (seq_len == seq_cap)
            {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq_terms[seq_len++] = (seq_t)k;
        }
    }

    return seq_terms[n - 1];
}
