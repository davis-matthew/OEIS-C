/*
 * A095282  Primes whose binary-expansion ends with an even number of 1's.
 * https://oeis.org/A095282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9708 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A095282 – primes whose binary expansion ends with an even number of trailing 1's. */
/* The function iteratively searches for primes and keeps those for which the count of
   trailing 1 bits is even (including the prime 2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}
#include <stddef.h>

static seq_t   *cache = NULL;
static size_t  len   = 0;
static size_t  cap   = 0;

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for indices < 1, but avoid UB */

    /* Build up the cache until it contains the required term. */
    if (len < n) {
        /* Seed the cache with the first term 2. */
        if (len == 0) {
            append_candidate(&cache, &len, &cap, (seq_t)2);
        }

        uint64_t last = (uint64_t)cache[len - 1];
        while (len < n) {
            uint64_t cand = next_prime(last);
            seq_t cand_seq = (seq_t)cand;

            /* Count the number of trailing 1 bits. */
            size_t ones = 0;
            seq_t tmp = cand_seq;
            while ((tmp & (seq_t)1) == (seq_t)1) {
                ++ones;
                tmp >>= 1;
            }

            /* Accept primes with an even count of trailing 1s. */
            if ((ones & 1) == 0) {
                append_candidate(&cache, &len, &cap, cand_seq);
            }

            last = cand;
        }
    }

    return cache[n - 1];
}
