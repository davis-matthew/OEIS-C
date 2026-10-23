/*
 * A131261  a(0)=3; for n>0, a(n) = smallest odd prime not occurring earlier
 *          in the sequence such that a(n-1)+a(n) is a multiple of n.
 * https://oeis.org/A131261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3; for n>0, a(n) = smallest odd prime not occurring earlier
 *            in the sequence such that a(n-1)+a(n) is a multiple of n
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/next_prime.h"    /* uint64_t next_prime(uint64_t start); */

/* Static memoisation of the sequence */
static seq_t *seq_cache = NULL;   /* array of terms */
static idx_t  seq_cap   = 0;      /* capacity of seq_cache    */
static idx_t  seq_len   = 0;      /* number of elements stored */

/* Static list of primes that have already been used */
static uint64_t *used_cache = NULL; /* array of used primes (uint64_t) */
static idx_t    used_cap   = 0;
static idx_t    used_len   = 0;

/* Append a term to the sequence cache */
static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1 : 8;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* Append a prime to the used list (store as uint64_t for efficiency) */
static void append_used(uint64_t val)
{
    if (used_len == used_cap) {
        used_cap = used_cap ? used_cap << 1 : 8;
        used_cache = (uint64_t *)realloc(used_cache, used_cap * sizeof(uint64_t));
    }
    used_cache[used_len++] = val;
}

/* Linear search whether a prime has already been used */
static bool used_exists(uint64_t val)
{
    for (idx_t i = 0; i < used_len; ++i)
        if (used_cache[i] == val) return true;
    return false;
}

seq_t a(idx_t n)
{
    /* Initialise the tables on the first call */
    if (seq_len == 0) {
        append_seq((seq_t)3);
        append_used((uint64_t)3);
    }

    /* Generate terms up to the requested index */
    while (seq_len <= n) {
        idx_t   cur_idx = seq_len;                /* divisor for a(n)   */
        seq_t prev = seq_cache[seq_len - 1];

        uint64_t cand = 3;                        /* start with the first odd prime */
        for (;;) {
            if (!used_exists(cand)) {
                seq_t sum = prev + (seq_t)cand;
                if ((sum % (seq_t)cur_idx) == (seq_t)0)
                    break;
            }
            cand = next_prime(cand + 1);
        }

        append_seq((seq_t)cand);
        append_used(cand);
    }

    return seq_cache[n];
}
