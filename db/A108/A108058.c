/*
 * A108058  Numbers k such that 179*k + 181 is prime.
 * https://oeis.org/A108058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 4325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that 179*k + 181 is prime.
   a(n) = the nth such integer (a(1)=0).  
   The implementation generates candidates k ≥ 0, tests primality of
   179*k + 181 by trial division, and stores the accepted k values.
   Results are cached so that calls with arbitrary n are answered in
   ≤O(n) time. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isprime.h"

/* Cached results of the sequence. */
static seq_t *seq = NULL;
static idx_t seq_len = 0;
static idx_t seq_cap = 0;
static uint64_t next_candidate = 0;  /* next k to be tested */

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety: index 0 is undefined */

    /* Grow cache up to at least n terms. */
    while (seq_len < n) {
        /* Ensure capacity. */
        if (seq_len == seq_cap) {
            size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
            while (new_cap < seq_len + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* allocation failure */
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Find next k such that 179*k + 181 is prime. */
        while (1) {
            uint64_t val = 179ULL * next_candidate + 181ULL;
            if (isprime(val)) {
                seq[seq_len++] = (seq_t)next_candidate;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }

    return seq[n - 1];
}
