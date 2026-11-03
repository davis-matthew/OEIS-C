/*
 * A215154  Primes congruent to {1, 2, 3} mod 19.
 * https://oeis.org/A215154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to {1, 2, 3} mod 19
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 812 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the n-th prime that is congruent to 1, 2, or 3 modulo 19.
 * The sequence is indexed starting at 1.
 */
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
    if (n == 0) return 0;            /* undefined for n<1, but safe */

    /* static state to memoise previously found primes */
    static seq_t *cache  = NULL;
    static size_t  size   = 0;
    static size_t  cap    = 0;
    static idx_t   last   = 1;      /* candidate value last tested */

    /* grow the cache until we have at least n entries */
    while (size < n)
    {
        /* find the next prime candidate */
        idx_t candidate;
        for (;;)
        {
            candidate = ++last;                /* move to next integer */
            if (candidate > 2 && (candidate & 1) == 0)   /* skip even >2 */
                continue;
            if (is_prime(candidate))
                break;                       /* prime found */
        }

        /* keep only primes with residue 1, 2, or 3 modulo 19 */
        idx_t r = candidate % 19;
        if (r == 1 || r == 2 || r == 3)
        {
            if (cap <= size)
                grow_seq(&cache, &cap, size + 1);  /* ensure storage */
            cache[size++] = (seq_t)candidate;     /* cast to seq_t */
        }
    }

    return cache[n - 1];   /* 1‑based index */
}
