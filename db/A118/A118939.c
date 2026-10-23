/*
 * A118939  Primes p such that (p^2+3)/4 is prime.
 * https://oeis.org/A118939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 37, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code finds a prime p such that (p^2+3)/4 is also prime.
   It enumerates primes in increasing order, tests the condition,
   and stores successful primes in a static cache so that subsequent
   calls are O(1) after the cache is built. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Cache of discovered sequence terms. */
    static seq_t *seq      = NULL;
    static size_t seq_cnt  = 0;
    static size_t seq_cap  = 0;

    /* Next integer to test for primality. */
    static idx_t next_candidate = 2;

    if (n == 0) return (seq_t)0;    /* 1‑based sequence; unused value */

    /* Expand the cache until it contains at least n terms. */
    while (seq_cnt < (size_t)n) {
        /* Find the next prime candidate. */
        seq_t cand = (seq_t)next_candidate;
        while (!is_prime_big(cand)) {
            ++next_candidate;
            cand = (seq_t)next_candidate;
        }
        seq_t p = cand;

        /* Compute t = (p^2+3)/4 using 128‑bit to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)p * (unsigned __int128)p;
        seq_t t = (seq_t)((prod + 3ULL) / 4ULL);

        if (is_prime_big(t)) {
            /* Grow the cache if necessary. */
            if (seq_cap <= seq_cnt) {
                size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(seq);
                    seq = NULL;
                    seq_cnt = 0;
                    seq_cap = 0;
                    return (seq_t)0;
                }
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_cnt++] = p;
        }

        /* Advance to the next integer for the next iteration. */
        ++next_candidate;
    }

    return seq[n - 1];
}
