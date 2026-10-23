/*
 * A015929  Positive integers n such that 2^n == 2^8 (mod n).
 * https://oeis.org/A015929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015929: a(n) is the n‑th positive integer m such that
 *          2^m ≡ 2^8 (mod m)   (i.e., 2^m ≡ 256 (mod m)).
 * The function computes terms by an incremental search of candidate
 * integers and caches each hit for fast repeated access. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* Return the n‑th element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Large static array to store discovered terms (more than 10 000
     * terms suffice for the required range).  seq_t is assumed to be
     * default‑constructible. */
    static seq_t cache[20000];
    static size_t cache_len = 0;       /* number of stored terms   */
    static uint64_t next_candidate = 1;/* integer to test next      */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        /* Compute 2^next_candidate modulo next_candidate. */
        seq_t pow_mod = mod_pow((seq_t)2,
                                (uint64_t)next_candidate,
                                (seq_t)next_candidate);

        /* 256 modulo the candidate.  This stays small. */
        seq_t target = (seq_t)(256 % next_candidate);

        /* If the congruence holds, remember the candidate. */
        if (pow_mod == target)
        {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* The terms are stored in 1‑based order, so return the (n‑1)-th. */
    return cache[n - 1];
}
