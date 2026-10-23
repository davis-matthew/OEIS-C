/*
 * A195271  1.5-gap primes: Prime p is a term iff there is no prime between
 *          1.5*p and 1.5*q, where q is the next prime after p.
 * https://oeis.org/A195271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p such that there is no prime strictly
 *            between ⌊3p/2⌋ and ⌊3q/2⌋, where q is the next prime after p
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Static storage used for memoisation */
static seq_t *seq = NULL;     /* array of found 1.5‑gap primes   */
static size_t seq_len = 0;    /* number of terms stored so far   */
static size_t seq_cap = 0;    /* capacity of seq array            */
static uint64_t curr_prime = 0;  /* next prime to be tested */

/* Ensure the capacity of the seq array. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0, but safe */

    /* Generate primes until we have at least n terms */
    while (seq_len < (size_t)n) {
        if (curr_prime == 0) curr_prime = 2ULL;   /* initialise to first prime */

        uint64_t p = curr_prime;
        uint64_t q = next_prime(p);               /* next prime after p     */

        /* Compute floor(3p/2) and floor(3q/2) safely */
        uint64_t lower = p + p / 2;   /* ⌊3p/2⌋ */
        uint64_t upper = q + q / 2;   /* ⌊3q/2⌋ */

        uint64_t next_after_lower = next_prime(lower);
        if (next_after_lower > upper) {
            ensure_seq_cap(seq_len + 1);
            seq[seq_len++] = (seq_t)p;
        }

        curr_prime = q;              /* next candidate prime   */
    }

    return seq[n - 1];
}
