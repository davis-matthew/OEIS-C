/*
 * A136191  Primes p such that 2p-3 and 2p+3 are both prime (A092110), with
 *          last decimal being 3.
 * https://oeis.org/A136191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A136191: primes p for which 2p-3 and 2p+3 are also prime,
   and p ends with digit 3 (OEIS offset 1).  This function
   generates the sequence lazily and memoises results. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined below offset */

    /* static memoisation arrays */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;
    static seq_t last_p    = 0;          /* last prime found in sequence */

    /* initialise on first call */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq[0] = 13;                     /* first term of the sequence */
        seq_len = 1;
        last_p = 13;
    }

    /* extend sequence until it contains at least n terms */
    while (seq_len < n) {
        /* next candidate ending with 3 */
        seq_t cand = last_p + (seq_t)10;
        while (!is_prime_big(cand))
            cand += (seq_t)10;            /* skip non‑primes */

        /* check the twin‑prime‑like conditions */
        seq_t two = cand + cand;          /* 2 * cand */
        if (!is_prime_big(two - (seq_t)3))
            { last_p = cand; continue; }
        if (!is_prime_big(two + (seq_t)3))
            { last_p = cand; continue; }

        /* accept the prime */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) exit(EXIT_FAILURE);
        }
        seq[seq_len++] = cand;
        last_p = cand;
    }

    return seq[n-1];
}
