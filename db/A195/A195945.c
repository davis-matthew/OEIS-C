/*
 * A195945  Powers of 13 which have no zero in their decimal expansion.
 * https://oeis.org/A195945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth power of 13 whose decimal representation contains
 *            no digit 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Return true iff x has no zero digit in base‑10 */
static bool no_zero_digit(seq_t x)
{
    if ((seq_t)0 == x)
        return false;           /* 0 contains a zero */
    while (x != (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)0)
            return false;
        x = x / (seq_t)10;
    }
    return true;
}

/* Cache of sequence terms (0‑indexed).  cache_len holds the number of valid terms stored. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Current power of 13 being examined (13^k, starting with k=0). */
static seq_t pow_next          = (seq_t)1;   /* 13^0 at first use */
static bool pow_next_initialized = false;

/* Return nth term of the sequence (n >= 1).  n==0 is undefined, returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset guard */

    if (!pow_next_initialized) {                /* initialise on first call */
        pow_next          = (seq_t)1;          /* 13^0 */
        pow_next_initialized = true;
    }

    /* Fill the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        if (no_zero_digit(pow_next)) {
            /* ensure space for a new term */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = pow_next;
        }
        /* move to next power of 13 */
        pow_next = pow_next * (seq_t)13;
    }

    return cache[(size_t)n - 1];
}
