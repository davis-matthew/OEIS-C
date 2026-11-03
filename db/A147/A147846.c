/*
 * A147846  Triangular numbers n*(n+1)/2 with n or n+1 prime.
 * https://oeis.org/A147846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9102 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A147846:
   Triangular numbers T_k = k*(k+1)/2 for k >= 1 such that
   k is prime or k+1 is prime. The sequence is indexed from n = 1. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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
