/*
 * A102649  Numbers n such that 11*n^2 + 11*n + 3 is prime.
 * https://oeis.org/A102649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that 11*k^2 + 11*k + 3 is prime
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence incrementally, caching all computed terms. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

static seq_t *terms = NULL;      /* cached sequence values */
static idx_t  terms_len = 0;     /* number of cached terms */
static idx_t  terms_cap = 0;     /* current capacity of terms */
static uint64_t next_n = 0;      /* next integer to test for primeness */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; behaviour for n=0 is irrelevant */

    /* Extend the cache until we have at least n terms. */
    while (terms_len < n) {
        /* Compute 11*next_n^2 + 11*next_n + 3 without overflow. */
        unsigned __int128 val = (unsigned __int128)11 * next_n * next_n
                              + (unsigned __int128)11 * next_n + 3;
        uint64_t expr = (uint64_t)val; /* fits into 64‑bit for the required n */

        if (is_prime_u64(expr))
            push_seq(&terms, &terms_cap, &terms_len, (seq_t)next_n);

        ++next_n;   /* advance to the next integer to test */
    }

    return terms[n - 1];
}
