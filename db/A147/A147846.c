/*
 * A147846  Triangular numbers n*(n+1)/2 with n or n+1 prime.
 * https://oeis.org/A147846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A147846:
   Triangular numbers T_k = k*(k+1)/2 for k >= 1 such that
   k is prime or k+1 is prime. The sequence is indexed from n = 1. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* treat offset 0 as 0 for safety */
    if (n == 0) return 0;

    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  cur_k = 1;   /* next k to examine */

    while (len < n) {
        /* find the next k for which k or k+1 is prime */
        for (;; ++cur_k) {
            if (is_prime(cur_k) || is_prime(cur_k + 1))
                break;
        }

        /* triangular number */
        seq_t val = (seq_t)cur_k * ((seq_t)cur_k + 1) / 2;

        /* enlarge cache if necessary */
        if (len == cap)
            grow_seq(&cache, &cap, len + 1);

        cache[len++] = val;

        /* advance cur_k for the next search */
        ++cur_k;
    }

    return cache[n - 1];
}
