/*
 * A050622  Numbers m that are divisible by 2^k, where k is the digit length
 *          of m.
 * https://oeis.org/A050622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9473 (2026-11-03)
 * Helpers:   ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050622: numbers divisible by 2^k where k is the digit length of the number. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Argument n is 1‑based (offset 1). */
    if (n == 0) return (seq_t)0;

    static seq_t *cache    = NULL;   /* cached sequence terms   */
    static size_t cache_len = 0;     /* number of terms cached  */
    static size_t cache_cap = 0;     /* current capacity        */
    static idx_t  current_k = 1;     /* current digit length     */
    static seq_t  next_val  = 0;     /* next value for current_k */
    static bool   initialized = false;

    /* Grow cache to at least n terms (roughly). */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n * 2;
        if (new_cap == 0) new_cap = 64;
        seq_t *newp = mem_resize(seq_t, cache, new_cap);
        if (!newp) exit(1);          /* allocation failure */
        cache     = newp;
        cache_cap = new_cap;
    }

    /* Initialise next_val for the first time. */
    if (!initialized) {
        seq_t lower = ipow_u64_idx(10ULL, current_k - 1); /* 10^(k-1)        */
        seq_t mult  = ipow_u64_idx(2ULL,  current_k);    /* 2^k              */
        seq_t rem   = lower % mult;
        next_val    = lower + (rem == 0 ? 0 : (mult - rem));
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t mult  = ipow_u64_idx(2ULL,  current_k);
        seq_t lower = ipow_u64_idx(10ULL, current_k - 1);
        seq_t upper = ipow_u64_idx(10ULL, current_k) - 1; /* 10^k-1         */

        cache[cache_len++] = next_val;   /* store current term   */
        next_val += mult;                /* advance to next one  */

        /* If next_val has passed the current digit-length range,
         * move to the next digit length. */
        if (next_val > upper) {
            current_k++;                                 /* new k        */
            seq_t lower_new = ipow_u64_idx(10ULL, current_k - 1);
            seq_t mult_new  = ipow_u64_idx(2ULL,  current_k);
            seq_t rem = lower_new % mult_new;
            next_val = lower_new + (rem == 0 ? 0 : (mult_new - rem));
        }
    }

    return cache[n - 1];
}
